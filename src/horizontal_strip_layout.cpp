#include "munin/horizontal_strip_layout.hpp"

#include "munin/component.hpp"

#include <algorithm>
#include <memory>
#include <numeric>

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent horizontal_strip_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints) const
{
    // The preferred size of the whole component is the maximum width of
    // the components and the sum of the preferred heights of the components.
    return std::accumulate(
        components.begin(),
        components.end(),
        terminalpp::extent{},
        [](auto preferred_size, auto const &comp) {
            auto const &comp_preferred_size = comp->get_preferred_size();

            preferred_size.width_ =
                (std::max)(preferred_size.width_, comp_preferred_size.width_);

            preferred_size.height_ += comp_preferred_size.height_;

            return preferred_size;
        });
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void horizontal_strip_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints,
    terminalpp::extent size) const
{
    auto y_coord = static_cast<terminalpp::coordinate_type>(0);

    std::for_each(
        components.begin(),
        components.end(),
        [&y_coord, size](auto const &comp) {
            auto preferred_size = comp->get_preferred_size();

            comp->set_position(terminalpp::point(0, y_coord));
            comp->set_size(
                terminalpp::extent(size.width_, preferred_size.height_));

            y_coord += preferred_size.height_;
        });
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json horizontal_strip_layout::do_to_json() const
{
    return {
        {"type", "horizontal_strip_layout"}
    };
}

// ==========================================================================
// MAKE_HORIZONTAL_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_horizontal_strip_layout()
{
    return std::make_unique<horizontal_strip_layout>();
}

}  // namespace munin
