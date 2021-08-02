#include "munin/horizontal_scrollbar.hpp"
#include "munin/detail/unicode_glyphs.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <boost/make_unique.hpp>
#include <memory>

namespace munin {

// ==========================================================================
// HORIZONTAL_SCROLLBAR IMPLEMENTATION STRUCTURE
// ==========================================================================
struct horizontal_scrollbar::impl
{
    terminalpp::coordinate_type viewport_x_position  = 0;
    terminalpp::coordinate_type viewport_basis_width = 0;
    boost::optional<terminalpp::coordinate_type> slider_position;

    void calculate_slider_position(
        terminalpp::coordinate_type const scrollbar_width)
    {
        if (viewport_basis_width == 0)
        {
            slider_position = boost::none;
        }
        else
        {
            // The slider is in the leftmost position only if the viewport 
            // x position is precisely 0.
            auto const &slider_is_in_leftmost_position =
                [=]
                {
                    return viewport_x_position == 0;
                };

            // The slider is in the rightmost position only if the viewport
            // basis is as far right as it can be.
            auto const &slider_is_in_rightmost_position =
                [=]
                {
                    return viewport_x_position == viewport_basis_width - 1;
                };

            auto const &interpolate_slider_position =
                [=]
                {
                    // There are scrollbar_width - 2 possible positions.
                    // The leftmost but one starts at 1, and increments
                    // by (scrollbar_width - 3) / (viewport_basis_width - 2)
                    // per viewport_x_position.
                    auto const slider_positions = scrollbar_width - 2;
                    auto const viewport_basis_positions = viewport_basis_width - 2;

                    // Starting from co-ordinate 1, increment by
                    // slider_positions / viewport_basis_width per viewport x
                    // position
                    auto const calculated_slider_position =  1 + (((viewport_x_position - 1) * slider_positions) 
                                / viewport_basis_positions);

                    std::cout << "calcualted position = " << calculated_slider_position << "\n";
                    return calculated_slider_position;
                };
            
            // Otherwise, it is an interpolated position between those points.
            slider_position =
                slider_is_in_leftmost_position() ? 0
              : slider_is_in_rightmost_position() ? (scrollbar_width - 1)
              : interpolate_slider_position();

            std::cout << "slider position = " << (slider_position ? *slider_position : -1) << "\n";
        }
    }
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
horizontal_scrollbar::horizontal_scrollbar()
  : pimpl_(boost::make_unique<impl>())
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
horizontal_scrollbar::~horizontal_scrollbar() = default;

// ==========================================================================
// SET_SLIDER_POSITION
// ==========================================================================
void horizontal_scrollbar::set_slider_position(
    terminalpp::coordinate_type x_position,
    terminalpp::coordinate_type width)
{
    pimpl_->viewport_x_position = x_position;
    pimpl_->viewport_basis_width = width;
    pimpl_->calculate_slider_position(get_size().width_);

    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void horizontal_scrollbar::do_set_size(terminalpp::extent const &size)
{
    pimpl_->calculate_slider_position(size.width_);
    basic_component::do_set_size(size);
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent horizontal_scrollbar::do_get_preferred_size() const
{
    return {get_size().width_, 1};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void horizontal_scrollbar::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    terminalpp::for_each_in_region(
        surface,
        region,
        [this](terminalpp::element &elem, 
               terminalpp::coordinate_type column, 
               terminalpp::coordinate_type row)
        {
            std::cout << "draw elem " << column << "," << row << '\n';
            elem = 
                column == pimpl_->slider_position
              ? munin::detail::single_lined_cross
              : munin::detail::single_lined_horizontal_beam;
        });
}

// ==========================================================================
// MAKE_HORIZONTAL_SCROLLBAR
// ==========================================================================
std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar()
{
    return std::make_shared<horizontal_scrollbar>();
}

}