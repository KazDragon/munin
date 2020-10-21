#include "munin/list.hpp"

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent list::do_get_preferred_size() const
{
    return {};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void list::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
}

// ==========================================================================
// MAKE_LIST
// ==========================================================================
std::shared_ptr<list> make_list()
{
    return std::make_shared<list>();
}

    
}