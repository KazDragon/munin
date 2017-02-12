#include "munin/horizontal_strip_layout.hpp"
#include "munin/container.hpp"

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
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        maximum_preferred_size.width = (std::max)(
            maximum_preferred_size.width
          , preferred_size.width);

        maximum_preferred_size.height += preferred_size.height;
    }

    return maximum_preferred_size;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void horizontal_strip_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size)
{
    auto y_coord = odin::u32(0);

    for (auto comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        comp->set_position(terminalpp::point(0, y_coord));
        comp->set_size(terminalpp::extent(size.width, preferred_size.height));

        y_coord += preferred_size.height;
    }
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

