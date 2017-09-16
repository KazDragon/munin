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
    auto used_north = 0;
    auto used_south = 0;
    auto used_west  = 0;
    auto used_east  = 0;

    for (auto index = 0u; index < components.size(); ++index)
    {
        auto const &component      = components[index];
        auto const *hint_any       = boost::any_cast<heading>(&hints[index]);
        auto const hint            = hint_any ? *hint_any : heading::centre;
        auto const &preferred_size = component->get_preferred_size();

        auto const remaining_height =
            (std::max)(size.height - used_north - used_south, 0);
        auto const remaining_width  =
            (std::max)(size.width - used_west - used_east, 0);

        switch (hint)
        {
            default :
                // fall-through
            case heading::centre :
                component->set_position({used_west, used_north});
                component->set_size({remaining_width, remaining_height});
                break;

            case heading::north :
                component->set_position({used_west, 0});
                component->set_size({
                    remaining_width,
                    (std::min)(preferred_size.height, remaining_height)});
                used_north = preferred_size.height;
                break;

            case heading::south :
                component->set_position({
                    used_west,
                    (std::max)(size.height - preferred_size.height, 0)});
                component->set_size({
                    remaining_width,
                    (std::min)(preferred_size.height, remaining_height)});
                used_south = preferred_size.height;
                break;

            case heading::west :
                component->set_position({0, used_north});
                component->set_size({
                    (std::min)(preferred_size.width, remaining_width),
                    remaining_height});
                used_west = preferred_size.width;
                break;

            case heading::east :
                component->set_position({
                    (std::max)(size.width - preferred_size.width, 0),
                    used_north});
                component->set_size({
                    (std::min)(preferred_size.width, remaining_width),
                    remaining_height});
                used_east = preferred_size.width;
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

