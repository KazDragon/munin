#include "munin/compass_layout.hpp"
#include "munin/component.hpp"

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent compass_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    return {};
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void compass_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    for (auto &component : components)
    {
        component->get_preferred_size();
        component->set_position({0, 0});
        component->set_size(size);
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json compass_layout::do_to_json() const
{
    return {
        { "type", "compass_layout" },
    };
}

// ==========================================================================
// MAKE_COMPASS_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_compass_layout()
{
    return std::unique_ptr<layout>(new compass_layout);
}

}

