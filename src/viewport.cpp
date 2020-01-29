#include "munin/viewport.hpp"
#include <boost/make_unique.hpp>
#include <utility>

namespace munin {

// ==========================================================================
// VIEWPORT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct viewport::impl
{
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
viewport::viewport(std::shared_ptr<component> tracked_component)
  : pimpl_(boost::make_unique<impl>())
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
    return {};
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
