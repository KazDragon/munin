#include "munin/component.hpp"
#include <cassert>

namespace munin {

// ==========================================================================
// SET_POSITION
// ==========================================================================
void component::set_position(terminalpp::point const &position)
{
    do_set_position(position);
}

// ==========================================================================
// GET_POSITION
// ==========================================================================
terminalpp::point component::get_position() const
{
    return do_get_position();
}

// ==========================================================================
// SET_SIZE
// ==========================================================================
void component::set_size(terminalpp::extent const &size)
{
    assert(size.width >= 0);
    assert(size.height >= 0);
    do_set_size(size);
}

// ==========================================================================
// GET_SIZE
// ==========================================================================
terminalpp::extent component::get_size() const
{
    return do_get_size();
}

// ==========================================================================
// GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent component::get_preferred_size() const
{
    return do_get_preferred_size();
}

// ==========================================================================
// HAS_FOCUS
// ==========================================================================
bool component::has_focus() const
{
    return do_has_focus();
}

// ==========================================================================
// SET_FOCUS
// ==========================================================================
void component::set_focus()
{
    do_set_focus();
}

// ==========================================================================
// LOSE_FOCUS
// ==========================================================================
void component::lose_focus()
{
    do_lose_focus();
}

// ==========================================================================
// FOCUS_NEXT
// ==========================================================================
void component::focus_next()
{
    do_focus_next();
}

// ==========================================================================
// FOCUS_PREVIOUS
// ==========================================================================
void component::focus_previous()
{
    do_focus_previous();
}

// ==========================================================================
// ENABLE
// ==========================================================================
void component::enable()
{
    do_enable();
}

// ==========================================================================
// DISABLE
// ==========================================================================
void component::disable()
{
    do_disable();
}

// ==========================================================================
// IS_ENABLED
// ==========================================================================
bool component::is_enabled() const
{
    return do_is_enabled();
}

// ==========================================================================
// GET_CURSOR_STATE
// ==========================================================================
bool component::get_cursor_state() const
{
    return do_get_cursor_state();
}

// ==========================================================================
// GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point component::get_cursor_position() const
{
    return do_get_cursor_position();
}

// ==========================================================================
// SET_CURSOR_POSITION
// ==========================================================================
void component::set_cursor_position(terminalpp::point const &position)
{
    do_set_cursor_position(position);
}

// ==========================================================================
// SET_ATTRIBUTE
// ==========================================================================
void component::set_attribute(std::string const &name, boost::any const &attr)
{
    do_set_attribute(name, attr);
}

// ==========================================================================
// LAYOUT
// ==========================================================================
void component::layout()
{
    do_layout();
}

// ==========================================================================
// DRAW
// ==========================================================================
void component::draw(
    context         &ctx
  , rectangle const &region) const
{
    do_draw(ctx, region);
}

// ==========================================================================
// EVENT
// ==========================================================================
void component::event(boost::any const &ev)
{
    do_event(ev);
}

}
