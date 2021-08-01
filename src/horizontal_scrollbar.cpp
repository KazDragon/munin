#include "munin/horizontal_scrollbar.hpp"
#include "munin/detail/unicode_glyphs.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <memory>

namespace munin {

terminalpp::extent horizontal_scrollbar::do_get_preferred_size() const
{
    return {get_size().width_, 1};
}

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
            elem = munin::detail::single_lined_horizontal_beam;
        });
}

std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar()
{
    return std::make_shared<horizontal_scrollbar>();
}

}