#include "munin/filled_box.hpp"

namespace munin { namespace detail {

// ==========================================================================
// MAKE_TOP_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_left_corner_fill(
    terminalpp::attribute *&attr);

// ==========================================================================
// MAKE_HORIZONTAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_horizontal_beam_fill(
    terminalpp::attribute *&attr);

// ==========================================================================
// MAKE_TOP_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_right_corner_fill(
    terminalpp::attribute *&attr);

// ==========================================================================
// MAKE_VERTICAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_vertical_beam_fill(
    terminalpp::attribute *&attr);

// ==========================================================================
// MAKE_BOTTOM_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_left_corner_fill(
    terminalpp::attribute *&attr);

// ==========================================================================
// MAKE_BOTTOM_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_right_corner_fill(
    terminalpp::attribute *&attr);

}}
