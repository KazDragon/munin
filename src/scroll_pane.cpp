#include "munin/scroll_pane.hpp"
#include "munin/solid_frame.hpp"

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_pane::scroll_pane(std::shared_ptr<component> const &inner_component)
  : framed_component(make_solid_frame(), inner_component)
{

}

// ==========================================================================
// MAKE_SCROLL_PANE
// ==========================================================================
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<scroll_pane>(inner_component);
}

}
