#include "munin/viewport.hpp"

#include "munin/render_surface.hpp"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/scope_exit.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <algorithm>
#include <memory>
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
    impl(
        viewport &self,
        std::shared_ptr<component> tracked_component,
        std::unique_ptr<viewport::resize_strategy> strategy)
      : self_(self),
        resize_strategy_(std::move(strategy)),
        tracked_component_(std::move(tracked_component))
    {
        tracked_component_->on_preferred_size_changed.connect(
            [this] { on_tracked_component_preferred_size_changed(); });
        tracked_component_->on_cursor_state_changed.connect(
            self_.on_cursor_state_changed);
        tracked_component_->on_cursor_position_changed.connect(
            [this] { on_tracked_component_cursor_position_changed(); });
        tracked_component_->on_redraw.connect([this](auto const &regions) {
            this->on_tracked_component_redraw(regions);
        });
        tracked_component_->on_focus_set.connect(self_.on_focus_set);
        tracked_component_->on_focus_lost.connect(self_.on_focus_lost);
    }

    // ======================================================================
    // GET_ANCHOR_BOUNDS
    // ======================================================================
    [[nodiscard]] auto get_anchor_bounds() const
    {
        return anchor_bounds_;
    }

    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    [[nodiscard]] auto get_preferred_size() const
    {
        return tracked_component_->get_preferred_size();
    }

    // ======================================================================
    // HAS_FOCUS
    // ======================================================================
    [[nodiscard]] auto has_focus() const
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
    [[nodiscard]] auto get_cursor_state() const
    {
        return tracked_component_->get_cursor_state();
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    [[nodiscard]] auto get_cursor_position() const
    {
        return cursor_position_;
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(render_surface &surface, terminalpp::rectangle const &region)
    {
        auto const offset_region = terminalpp::rectangle{
            region.origin_ + anchor_bounds_.origin_, region.size_};

        // The tracked component doesn't know it's in a viewport.  So, when
        // drawing some subset of its viewable space, we adjust the surface
        // so that it is aligned with the viewport.
        //
        // As an example, consider that the viewport is offset by (2, 2).
        // This means that (2, 2) in the tracked component is (0, 0) in the
        // viewport.  By offsetting by (-2, -2) (the negative of the anchor
        // position), the tracked component draws in the correct space.
        surface.offset_by(
            {-anchor_bounds_.origin_.x_, -anchor_bounds_.origin_.y_});

        // Ensure that the offset is unapplied before exit of this
        // function.
        BOOST_SCOPE_EXIT_ALL(&surface, this)
        {
            surface.offset_by(
                {anchor_bounds_.origin_.x_, anchor_bounds_.origin_.y_});
        };

        tracked_component_->draw(surface, offset_region);
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    auto event(std::any const &ev)
    {
        if (auto const *mouse_event =
                std::any_cast<terminalpp::mouse::event>(&ev);
            mouse_event)
        {
            auto const translated_event = terminalpp::mouse::event{
                mouse_event->action_,
                mouse_event->position_ + anchor_bounds_.origin_};

            return tracked_component_->event(translated_event);
        }
        else if (auto const *keypress_event =
                     std::any_cast<terminalpp::virtual_key>(&ev);
                 keypress_event)
        {
            if (keypress_event->key == terminalpp::vk::pgup)
            {
                auto const viewport_height = self_.get_size().height_;
                auto const cursor_position =
                    tracked_component_->get_cursor_position();

                tracked_component_->set_cursor_position(
                    {cursor_position.x_, cursor_position.y_ - viewport_height});
            }
            else if (keypress_event->key == terminalpp::vk::pgdn)
            {
                auto const viewport_height = self_.get_size().height_;
                auto const cursor_position =
                    tracked_component_->get_cursor_position();

                tracked_component_->set_cursor_position(
                    {cursor_position.x_, cursor_position.y_ + viewport_height});
            }
            else
            {
                tracked_component_->event(ev);
            }
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
        tracked_component_->set_size(
            resize_strategy_->calculate_tracked_component_size(
                tracked_component_->get_preferred_size(), self_.get_size()));
    }

    // ======================================================================
    // UPDATE_ANCHOR_POSITION
    // ======================================================================
    void update_anchor_position()
    {
        auto const tracked_cursor_position =
            tracked_component_->get_cursor_position();
        auto const old_anchor_bounds = anchor_bounds_;
        auto const tracked_component_size = tracked_component_->get_size();
        auto const viewport_size = self_.get_size();

        // Because size changes first match up the size of the tracked
        // component with the size of the viewport, it must be the case that
        // the tracked component's size is at least as large as the viewport
        // itself.
        assert(tracked_component_size.width_ >= viewport_size.width_);
        assert(tracked_component_size.height_ >= viewport_size.height_);

        // If the viewport has changed its size, look to see if the tracked
        // component is contained entirely in the viewport.  If not, then
        // adjust the anchor appropriately.
        anchor_bounds_.size_ = {
            tracked_component_size.width_ - viewport_size.width_,
            tracked_component_size.height_ - viewport_size.height_};

        anchor_bounds_.origin_ = {
            std::min(anchor_bounds_.origin_.x_, anchor_bounds_.size_.width_),
            std::min(anchor_bounds_.origin_.y_, anchor_bounds_.size_.height_)};

        // Check to see if the tracked cursor has scrolled off any of the
        // edges of the viewport.  If so, then the anchor position must change
        // just enough to keep the cursor within the visual area.
        anchor_bounds_.origin_ = {
            viewport_size.width_ == 0
                ? 0
                : std::clamp(
                    anchor_bounds_.origin_.x_,
                    tracked_cursor_position.x_ - viewport_size.width_ + 1,
                    tracked_cursor_position.x_),
            viewport_size.height_ == 0
                ? 0
                : std::clamp(
                    anchor_bounds_.origin_.y_,
                    tracked_cursor_position.y_ - viewport_size.height_ + 1,
                    tracked_cursor_position.y_)};

        if (old_anchor_bounds != anchor_bounds_)
        {
            self_.on_anchor_bounds_changed();
            self_.on_redraw({
                terminalpp::rectangle{{}, self_.get_size()}
            });
        }
    }

    // ======================================================================
    // UPDATE_CURSOR_POSITION
    // ======================================================================
    void update_cursor_position()
    {
        auto const old_cursor_position = cursor_position_;
        auto const tracked_cursor_position =
            tracked_component_->get_cursor_position();

        cursor_position_ = {
            tracked_cursor_position.x_ - anchor_bounds_.origin_.x_,
            tracked_cursor_position.y_ - anchor_bounds_.origin_.y_};

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
        update_anchor_position();
        update_cursor_position();
    }

    // ======================================================================
    // ON_TRACKED_COMPONENT_PREFERRED_SIZE_CHANGED
    // ======================================================================
    void on_tracked_component_preferred_size_changed()
    {
        update_tracked_component_size();
        update_anchor_position();
        update_cursor_position();
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
            [this](terminalpp::rectangle const &region) {
                return terminalpp::rectangle{
                    {region.origin_.x_ - anchor_bounds_.origin_.x_,
                     region.origin_.y_ - anchor_bounds_.origin_.y_},
                    region.size_
                };
            };

        auto const clip_region =
            [viewport_size](terminalpp::rectangle const &region) {
                return terminalpp::rectangle{
                    {region.origin_.x_, region.origin_.y_},
                    {std::min(
                         region.size_.width_,
                     viewport_size.width_ - region.origin_.x_),
                     std::min(
                         region.size_.height_,
                     viewport_size.height_ - region.origin_.y_)}
                };
            };

        auto const region_is_in_viewable_area =
            [viewport_size](terminalpp::rectangle const &region) {
                return region.origin_.x_ < viewport_size.width_
                    && region.origin_.y_ < viewport_size.height_;
            };

        using boost::adaptors::filtered;
        using boost::adaptors::transformed;

        self_.on_redraw(boost::copy_range<std::vector<terminalpp::rectangle>>(
            regions | transformed(translate_region) | transformed(clip_region)
            | filtered(region_is_in_viewable_area)));
    }

    viewport &self_;
    std::unique_ptr<viewport::resize_strategy> resize_strategy_;
    std::shared_ptr<component> tracked_component_;
    terminalpp::rectangle anchor_bounds_;
    terminalpp::point cursor_position_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
viewport::viewport(std::shared_ptr<component> tracked_component)
  : viewport(
      std::move(tracked_component), make_default_viewport_resize_strategy())
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
viewport::viewport(
    std::shared_ptr<component> tracked_component,
    std::unique_ptr<resize_strategy> strategy)
  : pimpl_(std::make_unique<impl>(
      *this, std::move(tracked_component), std::move(strategy)))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
viewport::~viewport() = default;

// ==========================================================================
// GET_ANCHOR_BOUNDS
// ==========================================================================
terminalpp::rectangle viewport::get_anchor_bounds() const
{
    return pimpl_->get_anchor_bounds();
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void viewport::do_set_size(terminalpp::extent const &size)
{
    basic_component::do_set_size(size);
    pimpl_->update_tracked_component_size();
    pimpl_->update_anchor_position();
    pimpl_->update_cursor_position();
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
    render_surface &surface, terminalpp::rectangle const &region) const
{
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void viewport::do_event(std::any const &event)
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

// ==========================================================================
// MAKE_VIEWPORT
// ==========================================================================
std::shared_ptr<viewport> make_viewport(
    std::shared_ptr<component> tracked_component,
    std::unique_ptr<viewport::resize_strategy> strategy)
{
    return std::make_shared<viewport>(
        std::move(tracked_component), std::move(strategy));
}

namespace {

class default_viewport_resize_strategy : public viewport::resize_strategy
{
    // ======================================================================
    // CALCULATE_TRACKED_COMPONENT_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent calculate_tracked_component_size(
        terminalpp::extent tracked_preferred_size,
        terminalpp::extent viewport_size) const override
    {
        return {
            std::max(tracked_preferred_size.width_, viewport_size.width_),
            std::max(tracked_preferred_size.height_, viewport_size.height_)};
    }
};

// ==========================================================================
// VERTICAL_VIEWPORT_RESIZE_STRATEGY
// ==========================================================================
class vertical_viewport_resize_strategy : public viewport::resize_strategy
{
    // ======================================================================
    // CALCULATE_TRACKED_COMPONENT_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent calculate_tracked_component_size(
        terminalpp::extent tracked_preferred_size,
        terminalpp::extent viewport_size) const override
    {
        return {
            viewport_size.width_,
            std::max(tracked_preferred_size.height_, viewport_size.height_)};
    }
};

// ==========================================================================
// HORIZONTAL_VIEWPORT_RESIZE_STRATEGY
// ==========================================================================
class horizontal_viewport_resize_strategy : public viewport::resize_strategy
{
    // ======================================================================
    // CALCULATE_TRACKED_COMPONENT_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent calculate_tracked_component_size(
        terminalpp::extent tracked_preferred_size,
        terminalpp::extent viewport_size) const override
    {
        return {
            std::max(tracked_preferred_size.width_, viewport_size.width_),
            viewport_size.height_};
    }
};

}  // namespace

// ==========================================================================
// MAKE_DEFAULT_VIEWPORT_RESIZE_STRATEGY
// ==========================================================================
std::unique_ptr<viewport::resize_strategy>
make_default_viewport_resize_strategy()
{
    return std::make_unique<default_viewport_resize_strategy>();
}

// ==========================================================================
// MAKE_VERTICAL_VIEWPORT_RESIZE_STRATEGY
// ==========================================================================
std::unique_ptr<viewport::resize_strategy>
make_vertical_viewport_resize_strategy()
{
    return std::make_unique<vertical_viewport_resize_strategy>();
}

// ==========================================================================
// MAKE_HORIZONTAL_VIEWPORT_RESIZE_STRATEGY
// ==========================================================================
std::unique_ptr<viewport::resize_strategy>
make_horizontal_viewport_resize_strategy()
{
    return std::make_unique<horizontal_viewport_resize_strategy>();
}

}  // namespace munin
