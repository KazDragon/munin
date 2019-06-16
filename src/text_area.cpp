#include "munin/text_area.hpp"

namespace munin {

// ==========================================================================
// ==========================================================================
terminalpp::extent text_area::do_get_preferred_size() const
{
    return {};
}

// ==========================================================================
// ==========================================================================
void text_area::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
}

// ==========================================================================
// ==========================================================================
std::shared_ptr<text_area> make_text_area()
{
    return std::make_shared<text_area>();
}

}