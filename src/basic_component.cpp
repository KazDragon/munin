#include "munin/basic_component.hpp"

#include "munin/detail/json_adaptors.hpp"

#include <terminalpp/mouse.hpp>


namespace munin {

namespace {

// ==========================================================================
// TOGGLE_FOCUS
// ==========================================================================
void toggle_focus(bool &has_focus, component &comp)
{
    if (std::exchange(has_focus, !has_focus))
    {
        comp.on_focus_lost();
    }
    else
    {
        comp.on_focus_set();
    }
}

}  // namespace

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
basic_component::basic_component() : has_focus_(false)
{
}

// ==========================================================================
// CAN_RECEIVE_FOCUS
// ==========================================================================
bool basic_component::can_receive_focus() const
{
    return do_can_receive_focus();
}

// ==========================================================================
// DO_CAN_RECEIVE_FOCUS
// ==========================================================================
bool basic_component::do_can_receive_focus() const
{
    return true;
}

// ==========================================================================
// DO_SET_POSITION
// ==========================================================================
void basic_component::do_set_position(terminalpp::point const &position)
{
    bounds_.origin_ = position;
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point basic_component::do_get_position() const
{
    return bounds_.origin_;
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void basic_component::do_set_size(terminalpp::extent const &size)
{
    bounds_.size_ = size;
}

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent basic_component::do_get_size() const
{
    return bounds_.size_;
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool basic_component::do_has_focus() const
{
    return has_focus_;
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void basic_component::do_set_focus()
{
    if (can_receive_focus())
    {
        if (!std::exchange(has_focus_, true))
        {
            on_focus_set();
        }
    }
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void basic_component::do_lose_focus()
{
    if (std::exchange(has_focus_, false))
    {
        on_focus_lost();
    }
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void basic_component::do_focus_next()
{
    if (can_receive_focus())
    {
        toggle_focus(has_focus_, *this);
    }
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void basic_component::do_focus_previous()
{
    if (can_receive_focus())
    {
        toggle_focus(has_focus_, *this);
    }
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool basic_component::do_get_cursor_state() const
{
    // By default, a component has no cursor.
    return false;
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point basic_component::do_get_cursor_position() const
{
    // By default, a component has no cursor, so we choose a sentinel
    // value of (0,0) for its non-existent location.
    return {};
}

// ==========================================================================
// DO_SET_CURSOR_POSITION
// ==========================================================================
void basic_component::do_set_cursor_position(terminalpp::point const &position)
{
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void basic_component::do_event(boost::any const &event)
{
    if (auto const *mouse = boost::any_cast<terminalpp::mouse::event>(&event);
        mouse != nullptr
        && mouse->action_ != terminalpp::mouse::event_type::button_up)
    {
        if (!has_focus())
        {
            set_focus();
        }
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json basic_component::do_to_json() const
{
    return {
        {"type",            "basic_component"                     },
        {"position",        detail::to_json(get_position())       },
        {"size",            detail::to_json(get_size())           },
        {"preferred_size",  detail::to_json(get_preferred_size()) },
        {"has_focus",       has_focus()                           },
        {"cursor_state",    get_cursor_state()                    },
        {"cursor_position", detail::to_json(get_cursor_position())}
    };
}

}  // namespace munin
