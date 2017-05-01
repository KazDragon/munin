#include "munin/layout.hpp"
#include <cassert>

namespace munin {

// ==========================================================================
// GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent layout::get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    assert(hints.size() == 0 || hints.size() == components.size());
    return do_get_preferred_size(components, hints);
}

// ==========================================================================
// OPERATOR()
// ==========================================================================
void layout::operator()(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size)
{
    assert(hints.size() == 0 || hints.size() == components.size());
    do_layout(components, hints, size);
}

}
