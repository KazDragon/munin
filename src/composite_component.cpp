#include <munin/composite_component.hpp>
#include <munin/container.hpp>

namespace munin {
    
// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
composite_component::composite_component()
{
    content_.on_redraw.connect(on_redraw);
    content_.on_preferred_size_changed.connect(on_preferred_size_changed);
    content_.on_focus_set.connect(on_focus_set);
    content_.on_focus_lost.connect(on_focus_lost);
    content_.on_cursor_state_changed.connect(on_cursor_state_changed);
    content_.on_cursor_position_changed.connect(on_cursor_position_changed);
}

// ==========================================================================
// SET_LAYOUT
// ==========================================================================
void composite_component::set_layout(std::unique_ptr<layout> lyt)
{
    content_.set_layout(std::move(lyt));
}

// ==========================================================================
// ADD_COMPONENT
// ==========================================================================
void composite_component::add_component(
    std::shared_ptr<component> const &comp,
    boost::any const &hint)
{
    content_.add_component(comp, hint);
}

// ==========================================================================
// DO_SET_POSITION
// ==========================================================================
void composite_component::do_set_position(terminalpp::point const &position)
{
    content_.set_position(position);
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point composite_component::do_get_position() const
{
    return content_.get_position();
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void composite_component::do_set_size(terminalpp::extent const &size)
{
    content_.set_size(size);
}

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent composite_component::do_get_size() const
{
    return content_.get_size();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent composite_component::do_get_preferred_size() const
{
    return content_.get_preferred_size();
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool composite_component::do_has_focus() const
{
    return content_.has_focus();
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void composite_component::do_set_focus()
{
    content_.set_focus();
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void composite_component::do_lose_focus()
{
    content_.lose_focus();
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void composite_component::do_focus_next()
{
    content_.focus_next();
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void composite_component::do_focus_previous()
{
    content_.focus_previous();
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool composite_component::do_get_cursor_state() const
{
    return content_.get_cursor_state();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point composite_component::do_get_cursor_position() const
{
    return content_.get_cursor_position();
}

// ==========================================================================
// DO_SET_CURSOR_POSITION
// ==========================================================================
void composite_component::do_set_cursor_position(terminalpp::point const &position)
{
    content_.set_cursor_position(position);
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void composite_component::do_draw(
    render_surface &surface,
    rectangle const &region) const
{
    content_.draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void composite_component::do_event(boost::any const &event)
{
    content_.event(event);
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json composite_component::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "composite_component" }
    ])"_json;

    return content_.to_json().patch(patch);
}

}
