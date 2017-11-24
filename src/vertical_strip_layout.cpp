#include "munin/vertical_strip_layout.hpp"
#include "munin/component.hpp"
#include <algorithm>
#include <numeric>

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent vertical_strip_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // The preferred size of the whole component is the maximum height of
    // the components and the sum of the preferred widths of the components.
    return std::accumulate(
        components.begin(),
        components.end(),
        terminalpp::extent{},
        [](auto preferred_size, auto const &comp)
    {
        auto const &comp_preferred_size = comp->get_preferred_size();
        
        preferred_size.height = (std::max)(
            preferred_size.height
          , comp_preferred_size.height);

        preferred_size.width += comp_preferred_size.width;
        
        return preferred_size;
    });
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void vertical_strip_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    auto x_coord = terminalpp::coordinate_type(0);

    std::for_each(components.begin(), components.end(),
        [&x_coord, size](auto const &comp)
        {
            auto preferred_size = comp->get_preferred_size();
    
            comp->set_position(terminalpp::point(x_coord, 0));
            comp->set_size(terminalpp::extent(preferred_size.width, size.height));
    
            x_coord += preferred_size.width;
        });
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json vertical_strip_layout::do_to_json() const
{
    return {
        { "type", "vertical_strip_layout" }
    };
}

// ==========================================================================
// MAKE_VERTICAL_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_vertical_strip_layout()
{
    return std::unique_ptr<vertical_strip_layout>(
        new vertical_strip_layout);
}

}
