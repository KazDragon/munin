#include "munin/aligned_layout.hpp"
#include "munin/container.hpp"
#include <algorithm>

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent aligned_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    return {};
    /*
    // The preferred size of this component is the largest preferred
    // extents of all components.
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto const &comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        maximum_preferred_size.width = (std::max)(
            maximum_preferred_size.width
          , preferred_size.width);

        maximum_preferred_size.height = (std::max)(
            maximum_preferred_size.height
          , preferred_size.height);
    }

    return maximum_preferred_size;
    */
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void aligned_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    for (uint32_t index = 0; index < components.size(); ++index)
    {
        auto const &comp = components[index];
        auto const &hint = hints[index];

        auto const *alignment_hint =
            boost::any_cast<alignment>(&hint);

        // By default, components are centre-aligned.
        alignment component_alignment = {
            horizontal_alignment::centre,
            vertical_alignment::centre
        };

        if (alignment_hint != nullptr)
        {
            component_alignment = *alignment_hint;
        }

        auto comp_size = comp->get_preferred_size();

        terminalpp::point position;

        switch (component_alignment.horizontal)
        {
        case horizontal_alignment::left :
            position.x = 0;
            break;

        case horizontal_alignment::right :
            position.x = comp_size.width > size.width
                       ? 0
                       : size.width - comp_size.width;
            break;

        case horizontal_alignment::centre :
        default :
            position.x = comp_size.width > size.width
                       ? 0
                       : ((size.width - comp_size.width) / 2);
            break;
        };

        switch (component_alignment.vertical)
        {
        case vertical_alignment::top :
            position.y = 0;
            break;

        case vertical_alignment::bottom :
            position.y = comp_size.height > size.height
                       ? 0
                       : size.height - comp_size.height;
            break;

        case vertical_alignment::centre :
        default :
            position.y = comp_size.height > size.height
                       ? 0
                       : ((size.height - comp_size.height) / 2);
            break;
        };

        comp->set_position(position);
        comp->set_size(comp_size);
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json aligned_layout::do_to_json() const
{
    return {
        { "type", "aligned_layout" }
    };
}

// ==========================================================================
// MAKE_ALIGNED_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_aligned_layout()
{
    return std::unique_ptr<layout>(new aligned_layout);
}

}