#include "munin/scroll_frame.hpp"

namespace munin {

// ==========================================================================
// NORTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type scroll_frame::north_border_height() const
{
    return 0;
}

// ==========================================================================
// SOUTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type scroll_frame::south_border_height() const
{
    return 0;
}

// ==========================================================================
// WEST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type scroll_frame::west_border_width() const
{
    return 0;
}

// ==========================================================================
// EAST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type scroll_frame::east_border_width() const
{
    return 0;
}

// ==========================================================================
// DO_INNER_FOCUS_CHANGE
// ==========================================================================
void scroll_frame::do_inner_focus_changed()
{
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<scroll_frame> make_scroll_frame()
{
    return std::make_shared<scroll_frame>();
}

}