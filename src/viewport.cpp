#include "munin/viewport.hpp"
#include "munin/render_surface.hpp"
#include <boost/make_unique.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/scope_exit.hpp>
#include <utility>

namespace munin {

// ==========================================================================
// VIEWPORT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct viewport::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(viewport& self, std::shared_ptr<component> tracked_component)
      : self_(self),
        tracked_component_(std::move(tracked_component))
    {
        tracked_component_->on_preferred_size_changed.connect(
            [this]{on_tracked_component_preferred_size_changed();});
        tracked_component_->on_cursor_state_changed.connect(
            self_.on_cursor_state_changed);
        tracked_component_->on_cursor_position_changed.connect(
            [this]{on_tracked_component_cursor_position_changed();});
        tracked_component_->on_redraw.connect(
            [this](auto const &regions)
            {
                this->on_tracked_component_redraw(regions);
            });
        tracked_component_->on_focus_set.connect(self_.on_focus_set);
        tracked_component_->on_focus_lost.connect(self_.on_focus_lost);
    }
    
    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    auto get_preferred_size() const
    {
        return tracked_component_->get_preferred_size();
    }

    // ======================================================================
    // HAS_FOCUS
    // ======================================================================
    auto has_focus() const
    {
        return tracked_component_->has_focus();
    }

    // ======================================================================
    // SET_FOCUS
    // ======================================================================
    void set_focus()
    {
        tracked_component_->set_focus();
    }

    // ======================================================================
    // LOSE_FOCUS
    // ======================================================================
    void lose_focus()
    {
        tracked_component_->lose_focus();
    }

    // ======================================================================
    // FOCUS_NEXT
    // ======================================================================
    void focus_next()
    {
        tracked_component_->focus_next();
    }

    // ======================================================================
    // FOCUS_PREVIOUS
    // ======================================================================
    void focus_previous()
    {
        tracked_component_->focus_previous();
    }

    // ======================================================================
    // GET_CURSOR_STATE
    // ======================================================================
    auto get_cursor_state() const
    {
        return tracked_component_->get_cursor_state();
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    auto get_cursor_position() const
    {
        return cursor_position_;
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(render_surface& surface, terminalpp::rectangle const &region)
    {
        auto const offset_region = terminalpp::rectangle{
            region.origin + anchor_position_,
            region.size
        };

        // The tracked component doesn't know it's in a viewport.  So, when
        // drawing some subset of its viewable space, we adjust the surface
        // so that it is aligned with the viewport.  
        //
        // As an example, consider that the viewport is offset by (2, 2).
        // This means that (2, 2) in the tracked component is (0, 0) in the
        // viewport.  By offsetting by (-2, -2) (the negative of the anchor
        // position), the tracked component draws in the correct space.
        surface.offset_by({
            -anchor_position_.x,
            -anchor_position_.y
        });

        // Ensure that the offset is unapplied before exit of this
        // function.
        BOOST_SCOPE_EXIT_ALL(&surface, this)
        {
            surface.offset_by({
                anchor_position_.x,
                anchor_position_.y
            });
        };

        tracked_component_->draw(surface, offset_region);
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    auto event(boost::any const& ev)
    {
        auto const *mouse_event = 
            boost::any_cast<terminalpp::ansi::mouse::report>(&ev);

        if (mouse_event != nullptr)
        {
            auto const translated_event = terminalpp::ansi::mouse::report {
                mouse_event->button_,
                mouse_event->x_position_ + anchor_position_.x,
                mouse_event->y_position_ + anchor_position_.y
            };

            return tracked_component_->event(translated_event);
        }
        else
        {
            return tracked_component_->event(ev);
        }
    }
    
    // ======================================================================
    // UPDATE_TRACKED_COMPONENT_SIZE
    // ======================================================================
    void update_tracked_component_size()
    {
        auto const preferred_size = tracked_component_->get_preferred_size();
        auto const viewport_size = self_.get_size();
        
        auto const tracked_component_size = terminalpp::extent{
            std::max(preferred_size.width, viewport_size.width),
            std::max(preferred_size.height, viewport_size.height)
        };
        
        tracked_component_->set_size(tracked_component_size);
    }

    // ======================================================================
    // UPDATE_VIEWPORT_POSITION
    // ======================================================================
    void update_viewport_position()
    {
        auto const old_cursor_position = cursor_position_;
        auto const old_viewport_position = anchor_position_;
        auto const tracked_cursor_position = tracked_component_->get_cursor_position();
        auto const viewport_size = self_.get_size();

        // Check to see if the tracked cursor has scrolled off an edge of the
        // viewport.  If so, then the anchor position must change just enough
        // to keep the cursor within the visual area.
        if (tracked_cursor_position.x < anchor_position_.x)
        {
            // Cursor has scrolled off to the west of the viewport.
            anchor_position_.x = tracked_cursor_position.x;
        }
        else if (tracked_cursor_position.x >= anchor_position_.x + viewport_size.width)
        {
            // Cursor has scrolled off to the east of the viewport.
            anchor_position_.x = (tracked_cursor_position.x - viewport_size.width) + 1;
        }

        if (tracked_cursor_position.y < anchor_position_.y)
        {
            anchor_position_.y = tracked_cursor_position.y;
        }
        else if (tracked_cursor_position.y >= anchor_position_.y + viewport_size.height)
        {
            // Cursor has scrolled off to the south of the viewport.
            anchor_position_.y = (tracked_cursor_position.y - viewport_size.height) + 1;
        }

        cursor_position_ = {
            tracked_cursor_position.x - anchor_position_.x,
            tracked_cursor_position.y - anchor_position_.y
        };

        if (old_viewport_position != anchor_position_)
        {
            self_.on_redraw({terminalpp::rectangle{{}, self_.get_size()}});
        }
        
        if (old_cursor_position != cursor_position_)
        {
            self_.on_cursor_position_changed();
        }
    }

private:
    // ======================================================================
    // ON_TRACKED_COMPONENT_CURSOR_POSITION_CHANGED
    // ======================================================================
    void on_tracked_component_cursor_position_changed()
    {
        update_viewport_position();
    }

    // ======================================================================
    // ON_TRACKED_COMPONENT_PREFERRED_SIZE_CHANGED
    // ======================================================================
    void on_tracked_component_preferred_size_changed()
    {
        update_tracked_component_size();
        self_.on_preferred_size_changed();
    }
    
    // ======================================================================
    // ON_TRACKED_COMPONENT_REDRAW
    // ======================================================================
    void on_tracked_component_redraw(
        std::vector<terminalpp::rectangle> const &regions)
    {
        auto const viewport_size = self_.get_size();

        auto const translate_region =
            [this](terminalpp::rectangle const &region)
            {
                return terminalpp::rectangle{
                    { 
                        region.origin.x - anchor_position_.x,
                        region.origin.y - anchor_position_.y
                    },
                    region.size
                };
            };

        auto const clip_region = 
            [viewport_size](terminalpp::rectangle const &region)
            {
                return terminalpp::rectangle {
                    { region.origin.x, region.origin.y },
                    {
                        std::min(
                            region.size.width, 
                            viewport_size.width - region.origin.x),
                        std::min(
                            region.size.height,
                            viewport_size.height - region.origin.y)
                    }
                };
            };

        auto const region_is_in_viewable_area =
            [viewport_size](terminalpp::rectangle const &region)
            {
                return region.origin.x < viewport_size.width
                    && region.origin.y < viewport_size.height;
            };

        using boost::adaptors::filtered;
        using boost::adaptors::transformed;

        self_.on_redraw(
            boost::copy_range<std::vector<terminalpp::rectangle>>(
                regions | transformed(translate_region)
                        | transformed(clip_region)
                        | filtered(region_is_in_viewable_area)
            ));
    }

    viewport &self_;
    std::shared_ptr<component> tracked_component_;
    terminalpp::point          anchor_position_;
    terminalpp::point          cursor_position_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
viewport::viewport(std::shared_ptr<component> tracked_component)
  : pimpl_(boost::make_unique<impl>(
        std::ref(*this), 
        std::move(tracked_component)))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
viewport::~viewport() = default;

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void viewport::do_set_size(terminalpp::extent const &size)
{
    basic_component::do_set_size(size);
    pimpl_->update_tracked_component_size();
    pimpl_->update_viewport_position();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent viewport::do_get_preferred_size() const
{
    return pimpl_->get_preferred_size();
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool viewport::do_has_focus() const
{
    return pimpl_->has_focus();
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void viewport::do_set_focus()
{
    pimpl_->set_focus();
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void viewport::do_lose_focus()
{
    pimpl_->lose_focus();
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void viewport::do_focus_next()
{
    pimpl_->focus_next();
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
void viewport::do_focus_previous()
{
    pimpl_->focus_previous();
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool viewport::do_get_cursor_state() const
{
    return pimpl_->get_cursor_state();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point viewport::do_get_cursor_position() const
{
    return pimpl_->get_cursor_position();
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void viewport::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void viewport::do_event(boost::any const &event)
{
    pimpl_->event(event);
}

// ==========================================================================
// MAKE_VIEWPORT
// ==========================================================================
std::shared_ptr<viewport> make_viewport(
    std::shared_ptr<component> tracked_component)
{
    return std::make_shared<viewport>(std::move(tracked_component));
}

}
