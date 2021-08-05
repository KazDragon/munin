#include "munin/scroll_frame.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/compass_layout.hpp"
#include "munin/horizontal_scrollbar.hpp"
#include "munin/vertical_scrollbar.hpp"
#include "munin/view.hpp"
#include <boost/make_unique.hpp>
namespace munin {

// ==========================================================================
// SCROLL_FRAME IMPLEMENTATION STRUCTURE
// ==========================================================================
struct scroll_frame::impl
{
    terminalpp::attribute current_attribute;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_frame::scroll_frame()
  : pimpl_(boost::make_unique<impl>())
{
    auto &attr = pimpl_->current_attribute;

    auto north_beam = view(
        make_compass_layout(),
        detail::make_top_left_corner_fill(attr), compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr), compass_layout::heading::centre,
        detail::make_top_right_corner_fill(attr),  compass_layout::heading::east);

    auto south_beam = view(
        make_compass_layout(),
        detail::make_bottom_left_corner_fill(attr), compass_layout::heading::west,
        make_horizontal_scrollbar(), compass_layout::heading::centre,
        detail::make_bottom_right_corner_fill(attr), compass_layout::heading::east);


    auto west_beam = detail::make_vertical_beam_fill(attr);
    auto east_beam = make_vertical_scrollbar();

    set_layout(make_compass_layout());

    add_component(north_beam, compass_layout::heading::north);
    add_component(south_beam, compass_layout::heading::south);
    add_component(west_beam, compass_layout::heading::west);
    add_component(east_beam, compass_layout::heading::east);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
scroll_frame::~scroll_frame() = default;

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