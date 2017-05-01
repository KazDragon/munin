#include "munin/vertical_squeeze_layout.hpp"
#include "munin/container.hpp"

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent vertical_squeeze_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // The preferred size of the whole component is the maximum preferred
    // height, and the maximum preferred width multiplied by the number
    // of components.
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto comp : components)
    {
        terminalpp::extent preferred_size = comp->get_preferred_size();

        maximum_preferred_size.width = (std::max)(
            maximum_preferred_size.width
          , preferred_size.width);

        maximum_preferred_size.height = (std::max)(
            maximum_preferred_size.height
          , preferred_size.height);
    }

    maximum_preferred_size.width *= components.size();

    return maximum_preferred_size;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void vertical_squeeze_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size)
{
    auto amount = components.size();

    if (amount != 0)
    {
        auto width_per_component =  size.width / amount;
        auto width_remainder =      size.width % amount;
        auto width_remainder_used = odin::u32(0);

        for (odin::u32 index = 0; index < amount; ++index)
        {
            auto comp = components[index];

            comp->set_position(terminalpp::point(
                (width_per_component * index) + width_remainder_used
                , odin::u32(0)));

            auto width = width_per_component;

            if (width_remainder != width_remainder_used)
            {
                ++width;
                ++width_remainder_used;
            }

            comp->set_size(terminalpp::extent(width, size.height));
        }
    }
}

// ==========================================================================
// MAKE_VERTICAL_SQUEEZE_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_vertical_squeeze_layout()
{
    return std::unique_ptr<vertical_squeeze_layout>(
        new vertical_squeeze_layout);
}

}

