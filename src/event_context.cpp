#include "munin/event_context.hpp"

namespace munin {

// ==========================================================================
// REGISTER_CLICK_INTEREST
// ==========================================================================
bool event_context::register_click_interest()
{
    has_click_interest_ = click_synthesis_supported_;
    return has_click_interest_;
}

// ==========================================================================
// DEREGISTER_CLICK_INTEREST
// ==========================================================================
void event_context::deregister_click_interest()
{
    has_click_interest_ = false;
}

// ==========================================================================
// SET_CLICK_SYNTHESIS_SUPPORTED
// ==========================================================================
void event_context::set_click_synthesis_supported(bool supported)
{
    click_synthesis_supported_ = supported;
}

// ==========================================================================
// RESET_MOUSE_DISPATCH_STATE
// ==========================================================================
void event_context::reset_mouse_dispatch_state()
{
    has_click_interest_ = false;
}

// ==========================================================================
// HAS_CLICK_INTEREST
// ==========================================================================
bool event_context::has_click_interest() const
{
    return has_click_interest_;
}

}  // namespace munin
