#include "munin/aligned_layout.hpp"

#include "munin/component.hpp"

#include <boost/range/numeric.hpp>

#include <algorithm>

namespace munin {

namespace {

// ==========================================================================
// CALCULATE_COMPONENT_WIDTH
// ==========================================================================
terminalpp::coordinate_type calculate_component_width(
    terminalpp::coordinate_type container_width,
    terminalpp::coordinate_type component_preferred_width,
    horizontal_alignment alignment)
{
    switch (alignment)
    {
        case horizontal_alignment::fill:
            return container_width;

        default:
            return std::min(container_width, component_preferred_width);
    }
}

// ==========================================================================
// CALCULATE_COMPONENT_HEIGHT
// ==========================================================================
terminalpp::coordinate_type calculate_component_height(
    terminalpp::coordinate_type container_height,
    terminalpp::coordinate_type component_preferred_height,
    vertical_alignment alignment)
{
    switch (alignment)
    {
        case vertical_alignment::fill:
            return container_height;

        default:
            return std::min(container_height, component_preferred_height);
    }
}

// ==========================================================================
// CALCULATE_COMPONENT_SIZE
// ==========================================================================
terminalpp::extent calculate_component_size(
    terminalpp::extent container_size,
    terminalpp::extent component_preferred_size,
    alignment alignment)
{
    return {
        calculate_component_width(
            container_size.width_,
            component_preferred_size.width_,
            alignment.horizontal),
        calculate_component_height(
            container_size.height_,
            component_preferred_size.height_,
            alignment.vertical)};
}

// ==========================================================================
// CALCULATE_COMPONENT_X_POSITION
// ==========================================================================
terminalpp::coordinate_type calculate_component_x_position(
    terminalpp::coordinate_type container_width,
    terminalpp::coordinate_type component_width,
    horizontal_alignment alignment)
{
    switch (alignment)
    {
        case horizontal_alignment::fill:
            // Fall-through
        case horizontal_alignment::left:
            return 0;

        case horizontal_alignment::right:
            return std::max(container_width - component_width, 0);

        case horizontal_alignment::centre:
        default:
            return std::max(container_width - component_width, 0) / 2;
    }
}

// ==========================================================================
// CALCULATE_COMPONENT_Y_POSITION
// ==========================================================================
terminalpp::coordinate_type calculate_component_y_position(
    terminalpp::coordinate_type container_height,
    terminalpp::coordinate_type component_height,
    vertical_alignment alignment)
{
    switch (alignment)
    {
        case vertical_alignment::fill:
            // Fall-through
        case vertical_alignment::top:
            return 0;

        case vertical_alignment::bottom:
            return std::max(container_height - component_height, 0);

        case vertical_alignment::centre:
        default:
            return std::max(container_height - component_height, 0) / 2;
    }
}

// ==========================================================================
// CALCULATE_COMPONENT_POSITION
// ==========================================================================
terminalpp::point calculate_component_position(
    terminalpp::extent container_size,
    terminalpp::extent component_size,
    alignment component_alignment)
{
    return {
        calculate_component_x_position(
            container_size.width_,
            component_size.width_,
            component_alignment.horizontal),
        calculate_component_y_position(
            container_size.height_,
            component_size.height_,
            component_alignment.vertical),
    };
}

// ==========================================================================
// CALCULATE_ALIGNMENT
// ==========================================================================
alignment calculate_alignment(boost::any const &hint)
{
    auto const *alignment_hint = boost::any_cast<alignment>(&hint);

    // By default, components are centre-aligned.
    return alignment_hint != nullptr
             ? *alignment_hint
             : alignment{
                 horizontal_alignment::centre, vertical_alignment::centre};
}

}  // namespace

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent aligned_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any> const &hints) const
{
    // The preferred size of this component is the largest preferred
    // extents of all components.
    return boost::accumulate(
        components,
        terminalpp::extent{0, 0},
        [](terminalpp::extent max, std::shared_ptr<component> const &comp) {
            auto const preferred_size = comp->get_preferred_size();

            return terminalpp::extent{
                std::max(preferred_size.width_, max.width_),
                std::max(preferred_size.height_, max.height_)};
        });
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void aligned_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any> const &hints,
    terminalpp::extent size) const
{
    for (uint32_t index = 0; index < components.size(); ++index)
    {
        auto const &comp = components[index];
        auto const &hint = hints[index];
        auto const &alignment = calculate_alignment(hint);

        auto const &component_size = calculate_component_size(
            size, comp->get_preferred_size(), alignment);

        auto const &component_position =
            calculate_component_position(size, component_size, alignment);

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
        {"type", "aligned_layout"}
    };
}

// ==========================================================================
// MAKE_ALIGNED_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_aligned_layout()
{
    return std::unique_ptr<layout>(new aligned_layout);
}

}  // namespace munin
