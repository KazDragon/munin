#include "munin/edit.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent edit::do_get_preferred_size() const
{
    return {1, 1};
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool edit::do_get_cursor_state() const
{
    return true;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void edit::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    terminalpp::for_each_in_region(
        surface,
        region,
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = ' ';
        });
}

// ==========================================================================
// MAKE_EDIT
// ==========================================================================
std::shared_ptr<edit> make_edit()
{
    return std::make_shared<edit>();
}
    
}
