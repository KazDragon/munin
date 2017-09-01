#include "munin/aligned_layout.hpp"
#include "munin/container.hpp"
#include <algorithm>

namespace munin {

// ==========================================================================
// CALCULATE_COMPONENT_SIZE
// ==========================================================================
static terminalpp::extent calculate_component_size(
    terminalpp::extent const &container_size,
    terminalpp::extent const &preferred_component_size)
{
    return {
        (std::min)(preferred_component_size.width, container_size.width),
        (std::min)(preferred_component_size.height, container_size.height)
    };
}

// ==========================================================================
// CALCULATE_COMPONENT_POSITION
// ==========================================================================
static terminalpp::point calculate_component_position(
    terminalpp::extent const &container_size,
    terminalpp::extent const &component_size,
    alignment const &component_alignment)
{
    terminalpp::point position;
    
    switch (component_alignment.horizontal)
    {
    case horizontal_alignment::left :
        position.x = 0;
        break;

    case horizontal_alignment::right :
        position.x = 
            (std::max)(container_size.width - component_size.width, 0);
        break;

    case horizontal_alignment::centre :
    default :
        position.x =
            (std::max)(container_size.width - component_size.width, 0) / 2;
        break;
    };

    switch (component_alignment.vertical)
    {
    case vertical_alignment::top :
        position.y = 0;
        break;

    case vertical_alignment::bottom :
        position.y =
            (std::max)(container_size.height - component_size.height, 0);
        break;

    case vertical_alignment::centre :
    default :
        position.y =
            (std::max)(container_size.height - component_size.height, 0) / 2;
        break;
    };
    
    return position;
}

// ==========================================================================
// CALCULATE_ALIGNMENT
// ==========================================================================
static alignment calculate_alignment(boost::any const &hint)
{
    auto const *alignment_hint =
        boost::any_cast<alignment>(&hint);

    // By default, components are centre-aligned.
    return alignment_hint != nullptr
         ? *alignment_hint
         : alignment {
               horizontal_alignment::centre, 
               vertical_alignment::centre 
           };
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent aligned_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // The preferred size of this component is the largest preferred
    // extents of all components.
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto const &comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        maximum_preferred_size.width = (std::max)(
            maximum_preferred_size.width,
            preferred_size.width);

        maximum_preferred_size.height = (std::max)(
            maximum_preferred_size.height,
            preferred_size.height);
    }

    return maximum_preferred_size;
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

        auto const component_size = calculate_component_size(
            size, comp->get_preferred_size());

        auto const component_position = calculate_component_position(
            size, component_size, calculate_alignment(hint));

        comp->set_position(component_position);
        comp->set_size(component_size);
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
