#include "munin/horizontal_scrollbar.hpp"
#include <memory>

namespace munin {

terminalpp::extent horizontal_scrollbar::do_get_preferred_size() const
{
    return {};
}

void horizontal_scrollbar::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
}

std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar()
{
    return std::make_shared<horizontal_scrollbar>();
}

}