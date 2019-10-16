#include "munin/edit.hpp"

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent edit::do_get_preferred_size() const
{
    return {};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void edit::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
}

// ==========================================================================
// MAKE_EDIT
// ==========================================================================
std::shared_ptr<edit> make_edit()
{
    return std::make_shared<edit>();
}
    
}
