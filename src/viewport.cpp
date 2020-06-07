#include "munin/viewport.hpp"
#include <boost/make_unique.hpp>
#include <boost/range/adaptor/transformed.hpp>
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
        tracked_component_->on_cursor_position_changed.connect(
            [this]{on_tracked_component_cursor_position_changed();});
        tracked_component_->on_redraw.connect(
            [this](auto const &regions)
            {
                this->on_tracked_component_redraw(regions);
            });
    }
    
    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    auto get_preferred_size()
    {
        return tracked_component_->get_preferred_size();
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    auto get_cursor_position()
    {
        return cursor_position_;
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(render_surface& surface, terminalpp::rectangle const &region)
    {
        auto const offset_region = terminalpp::rectangle{
            region.origin + viewport_position_,
            region.size
        };

        tracked_component_->draw(surface, offset_region);
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    auto event(boost::any const& ev)
    {
        return tracked_component_->event(ev);
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
        auto const old_viewport_position = viewport_position_;
        auto const tracked_cursor_position = tracked_component_->get_cursor_position();
        auto const viewport_size = self_.get_size();

        // Check to see if the tracked cursor has scrolled off an edge of the
        // viewport.  If so, then the viewport position must change just enough
        // to keep the cursor within the visual area.
        if (tracked_cursor_position.x < viewport_position_.x)
        {
            // Cursor has scrolled off to the west of the viewport.
            viewport_position_.x = tracked_cursor_position.x;
        }
        else if (tracked_cursor_position.x >= viewport_position_.x + viewport_size.width)
        {
            // Cursor has scrolled off to the east of the viewport.
            viewport_position_.x = (tracked_cursor_position.x - viewport_size.width) + 1;
        }

        if (tracked_cursor_position.y < viewport_position_.y)
        {
            viewport_position_.y = tracked_cursor_position.y;
        }
        else if (tracked_cursor_position.y >= viewport_position_.y + viewport_size.height)
        {
            // Cursor has scrolled off to the south of the viewport.
            viewport_position_.y = (tracked_cursor_position.y - viewport_size.height) + 1;
        }

        cursor_position_ = {
            tracked_cursor_position.x - viewport_position_.x,
            tracked_cursor_position.y - viewport_position_.y
        };

        if (old_viewport_position != viewport_position_)
        {
            self_.on_redraw({terminalpp::rectangle{self_.get_position(), self_.get_size()}});
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
        self_.on_redraw(regions);
    }

    viewport &self_;
    std::shared_ptr<component> tracked_component_;
    terminalpp::point          viewport_position_;
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
