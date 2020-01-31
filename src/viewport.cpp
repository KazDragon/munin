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
    }
    
    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    auto get_preferred_size()
    {
        return tracked_component_->get_preferred_size();
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    auto event(boost::any const& ev)
    {
        return tracked_component_->event(ev);
    }
    
private:
    // ======================================================================
    // ON_TRACKED_COMPONENT_PREFERRED_SIZE_CHANGED
    // ======================================================================
    void on_tracked_component_preferred_size_changed()
    {
        tracked_component_->set_size(tracked_component_->get_preferred_size());
        self_.on_preferred_size_changed();
    }
    
    viewport &self_;
    std::shared_ptr<component> tracked_component_;
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
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent viewport::do_get_preferred_size() const
{
    return pimpl_->get_preferred_size();
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void viewport::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
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
