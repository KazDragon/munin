#include "munin/vertical_strip_layout.hpp"
#include "munin/container.hpp"

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
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto const &comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        maximum_preferred_size.height = (std::max)(
            maximum_preferred_size.height
          , preferred_size.height);

        maximum_preferred_size.width += preferred_size.width;
    }

    return maximum_preferred_size;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void vertical_strip_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    auto x_coord = terminalpp::u32(0);

    for (auto &comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        comp->set_position(terminalpp::point(x_coord, 0));
        comp->set_size(terminalpp::extent(preferred_size.width, size.height));

        x_coord += preferred_size.width;
    }
}

// ==========================================================================
// MAKE_vertical_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_vertical_strip_layout()
{
    return std::unique_ptr<vertical_strip_layout>(
        new vertical_strip_layout);
}

}

