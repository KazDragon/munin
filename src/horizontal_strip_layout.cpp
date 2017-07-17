#include "munin/horizontal_strip_layout.hpp"
#include "munin/container.hpp"
#include <algorithm>
#include <numeric>

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent horizontal_strip_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // The preferred size of the whole component is the maximum width of
    // the components and the sum of the preferred heights of the components.
    return std::accumulate(
        components.begin(),
        components.end(),
        terminalpp::extent{},
        [](auto preferred_size, auto const &comp)
    {
        auto const &comp_preferred_size = comp->get_preferred_size();
        
        preferred_size.width = (std::max)(
            preferred_size.width
          , comp_preferred_size.width);

        preferred_size.height += comp_preferred_size.height;
        
        return preferred_size;
    });
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void horizontal_strip_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    auto y_coord = terminalpp::u32(0);

    std::for_each(components.begin(), components.end(),
        [&y_coord, size](auto const &comp)
        {
            auto preferred_size = comp->get_preferred_size();
    
            comp->set_position(terminalpp::point(0, y_coord));
            comp->set_size(terminalpp::extent(size.width, preferred_size.height));
    
            y_coord += preferred_size.height;
        });
}

// ==========================================================================
// MAKE_HORIZONTAL_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_horizontal_strip_layout()
{
    return std::unique_ptr<horizontal_strip_layout>(
        new horizontal_strip_layout);
}

}
