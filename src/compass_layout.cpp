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
    for (auto index = 0u; index < components.size(); ++index)
    {
        auto const &component      = components[index];
        auto const *hint_any       = boost::any_cast<heading>(&hints[index]);
        auto const hint            = hint_any ? *hint_any : heading::centre;
        auto const &preferred_size = component->get_preferred_size();

        switch (hint)
        {
            default :
                // fall-through
            case heading::centre :
                component->set_position({0, 0});
                component->set_size(size);
                break;

            case heading::north :
                component->set_position({0, 0});
                component->set_size({size.width, preferred_size.height});
                break;

            case heading::south :
                component->set_position({0, size.height - preferred_size.height});
                component->set_size({size.width, preferred_size.height});
                break;

            case heading::west :
                component->set_position({0, 0});
                component->set_size({preferred_size.width, size.height});
                break;

            case heading::east :
                component->set_position({size.width - preferred_size.width, 0});
                component->set_size({preferred_size.width, size.height});
                break;
        }

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

