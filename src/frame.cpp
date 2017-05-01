#include "munin/frame.hpp"

namespace munin {

// ==========================================================================
// GET_TOP_BORDER_HEIGHT
// ==========================================================================
odin::s32 frame::get_top_border_height() const
{
    return do_get_top_border_height();
}

// ==========================================================================
// GET_BOTTOM_BORDER_HEIGHT
// ==========================================================================
odin::s32 frame::get_bottom_border_height() const
{
    return do_get_bottom_border_height();
}

// ==========================================================================
// GET_LEFT_BORDER_WIDTH
// ==========================================================================
odin::s32 frame::get_left_border_width() const
{
    return do_get_left_border_width();
}

// ==========================================================================
// GET_RIGHT_BORDER_WIDTH
// ==========================================================================
odin::s32 frame::get_right_border_width() const
{
    return do_get_right_border_width();
}

}
