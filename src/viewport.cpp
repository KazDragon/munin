#include "munin/viewport.hpp"
#include <boost/make_unique.hpp>
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
        tracked_component_->draw(surface, region);
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
private:
    // ======================================================================
    // ON_TRACKED_COMPONENT_CURSOR_POSITION_CHANGED
    // ======================================================================
    void on_tracked_component_cursor_position_changed()
    {
        auto const old_cursor_position = cursor_position_;
        cursor_position_ = tracked_component_->get_cursor_position();

        if (old_cursor_position != cursor_position_)
        {
            self_.on_cursor_position_changed();
        }
    }

    // ======================================================================
    // ON_TRACKED_COMPONENT_PREFERRED_SIZE_CHANGED
    // ======================================================================
    void on_tracked_component_preferred_size_changed()
    {
        update_tracked_component_size();
        self_.on_preferred_size_changed();
    }
    
    viewport &self_;
    std::shared_ptr<component> tracked_component_;
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
