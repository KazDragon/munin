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
    captured_component_.reset();
    captured_origin_ = {};
    captured_mouse_inside_ = false;
}

// ==========================================================================
// HAS_CLICK_INTEREST
// ==========================================================================
bool event_context::has_click_interest() const
{
    return has_click_interest_;
}

// ==========================================================================
// CAPTURE_MOUSE
// ==========================================================================
void event_context::capture_mouse(
    std::shared_ptr<component> const &component,
    terminalpp::point const &origin)
{
    captured_component_ = component;
    captured_origin_ = origin;
    captured_mouse_inside_ = true;
}

// ==========================================================================
// CAPTURED_COMPONENT
// ==========================================================================
std::shared_ptr<component> event_context::captured_component() const
{
    return captured_component_.lock();
}

// ==========================================================================
// CAPTURED_ORIGIN
// ==========================================================================
terminalpp::point event_context::captured_origin() const
{
    return captured_origin_;
}

// ==========================================================================
// CAPTURED_MOUSE_INSIDE
// ==========================================================================
bool event_context::captured_mouse_inside() const
{
    return captured_mouse_inside_;
}

// ==========================================================================
// SET_CAPTURED_MOUSE_INSIDE
// ==========================================================================
void event_context::set_captured_mouse_inside(bool inside)
{
    captured_mouse_inside_ = inside;
}

}  // namespace munin
