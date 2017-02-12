#include "munin/layout.hpp"

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
layout::layout()
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
layout::~layout()
{
}

// ==========================================================================
// GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent layout::get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
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
    do_layout(components, hints, size);
}

}

