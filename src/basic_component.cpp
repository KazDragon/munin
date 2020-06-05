#include "munin/basic_component.hpp"
#include "munin/detail/json_adaptors.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <map>

namespace munin {

// ==========================================================================
// BASIC_COMPONENT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct basic_component::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(basic_component &self)
        : self_(self)
        , has_focus_(false)
    {
    }

    // ======================================================================
    // TOGGLE_FOCUS
    // ======================================================================
    void toggle_focus()
    {
        if (has_focus_)
        {
            has_focus_ = false;
            self_.on_focus_lost();
        }
        else
        {
            has_focus_ = true;
            self_.on_focus_set();
        }
    }

    basic_component       &self_;
    terminalpp::rectangle bounds_;
    bool                  has_focus_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
basic_component::basic_component()
{
    pimpl_ = std::make_shared<impl>(std::ref(*this));
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
basic_component::~basic_component()
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
    pimpl_->bounds_.origin = position;
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point basic_component::do_get_position() const
{
    return pimpl_->bounds_.origin;
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void basic_component::do_set_size(terminalpp::extent const &size)
{
    pimpl_->bounds_.size = size;
}

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent basic_component::do_get_size() const
{
    return pimpl_->bounds_.size;
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool basic_component::do_has_focus() const
{
    return pimpl_->has_focus_;
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void basic_component::do_set_focus()
{
    if (can_receive_focus())
    {
        bool old_focus = pimpl_->has_focus_;
        pimpl_->has_focus_ = true;

        if (!old_focus)
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
    bool old_focus = pimpl_->has_focus_;
    pimpl_->has_focus_ = false;

    if (old_focus)
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
        pimpl_->toggle_focus();
    }
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void basic_component::do_focus_previous()
{
    if (can_receive_focus())
    {
        pimpl_->toggle_focus();
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
    // By default, a component has no cursor, so we choose a sentry
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
    auto const *mouse =
        boost::any_cast<terminalpp::ansi::mouse::report>(&event);

    if (mouse
     && mouse->button_ != terminalpp::ansi::mouse::report::BUTTON_UP)
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
        {"type",            "basic_component"},
        {"position",        detail::to_json(get_position())},
        {"size",            detail::to_json(get_size())},
        {"preferred_size",  detail::to_json(get_preferred_size())},
        {"has_focus",       has_focus()},
        {"cursor_state",    get_cursor_state()},
        {"cursor_position", detail::to_json(get_cursor_position())}
    };
}

}

