#include "munin/layout.hpp"

#include <cassert>

namespace munin {

// ==========================================================================
// GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent layout::get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints) const
{
    assert(hints.empty() || hints.size() == components.size());
    return do_get_preferred_size(components, hints);
}

// ==========================================================================
// OPERATOR()
// ==========================================================================
void layout::operator()(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<std::any> const &hints,
    terminalpp::extent size) const
{
    assert(hints.empty() || hints.size() == components.size());
    do_layout(components, hints, size);
}

// ==========================================================================
// TO_JSON
// ==========================================================================
nlohmann::json layout::to_json() const
{
    return do_to_json();
}

}  // namespace munin
