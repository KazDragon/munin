#include "munin/null_layout.hpp"

namespace munin {

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent null_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints) const
{
    return {};
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void null_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints,
    terminalpp::extent size) const
{
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json null_layout::do_to_json() const
{
    return {
        {"type", "null_layout"}
    };
}

// ==========================================================================
// MAKE_NULL_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_null_layout()
{
    return std::unique_ptr<layout>(new null_layout);
}

}  // namespace munin
