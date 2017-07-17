#include "munin/grid_layout.hpp"
#include "munin/component.hpp"
#include <numeric>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
grid_layout::grid_layout(terminalpp::extent size)
  : size_(size)
{
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent grid_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    auto max_preferred_sizes = std::accumulate(
        components.begin(),
        components.end(),
        terminalpp::extent{},
        [this](auto preferred_size, auto const &component)
    {
        auto const &component_preferred_size = component->get_preferred_size();
        preferred_size.width  = std::max(
            component_preferred_size.width, preferred_size.width);
        preferred_size.height = std::max(
            component_preferred_size.height, preferred_size.height);
            
        return preferred_size;
    });

    return {
        max_preferred_sizes.width  * size_.width,
        max_preferred_sizes.height * size_.height
    };
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void grid_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
}

// ==========================================================================
// MAKE_HORIZONTAL_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_grid_layout(terminalpp::extent size)
{
    return std::unique_ptr<layout>(new grid_layout(size));
}

}

