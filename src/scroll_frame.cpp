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
    terminalpp::attribute current_attribute_;
    std::shared_ptr<horizontal_scrollbar> horizontal_scrollbar_;
    std::shared_ptr<vertical_scrollbar> vertical_scrollbar_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_frame::scroll_frame()
  : pimpl_(boost::make_unique<impl>())
{
    pimpl_->horizontal_scrollbar_ = make_horizontal_scrollbar();
    pimpl_->vertical_scrollbar_ = make_vertical_scrollbar();

    auto &attr = pimpl_->current_attribute_;

    auto north_beam = view(
        make_compass_layout(),
        detail::make_top_left_corner_fill(attr), compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr), compass_layout::heading::centre,
        detail::make_top_right_corner_fill(attr),  compass_layout::heading::east);

    auto south_beam = view(
        make_compass_layout(),
        detail::make_bottom_left_corner_fill(attr), compass_layout::heading::west,
        pimpl_->horizontal_scrollbar_, compass_layout::heading::centre,
        detail::make_bottom_right_corner_fill(attr), compass_layout::heading::east);


    auto west_beam = detail::make_vertical_beam_fill(attr);
    auto east_beam = pimpl_->vertical_scrollbar_;

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
// SET_HORIZONTAL_SLIDER_POSITION
// ==========================================================================
void scroll_frame::set_horizontal_slider_position(
    terminalpp::coordinate_type x_position,
    terminalpp::coordinate_type width)
{
    pimpl_->horizontal_scrollbar_->set_slider_position(x_position, width);
}

// ==========================================================================
// SET_VERTICAL_SLIDER_POSITION
// ==========================================================================
void scroll_frame::set_vertical_slider_position(
    terminalpp::coordinate_type y_position,
    terminalpp::coordinate_type height)
{
    pimpl_->vertical_scrollbar_->set_slider_position(y_position, height);
}

// ==========================================================================
// NORTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type scroll_frame::north_border_height() const
{
    return 1;
}

// ==========================================================================
// SOUTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type scroll_frame::south_border_height() const
{
    return 1;
}

// ==========================================================================
// WEST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type scroll_frame::west_border_width() const
{
    return 1;
}

// ==========================================================================
// EAST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type scroll_frame::east_border_width() const
{
    return 1;
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