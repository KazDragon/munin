#include "paint/paint_canvas.hpp"

#include "paint/paint_model.hpp"

#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <munin/render_surface.hpp>

#include <algorithm>
#include <any>

namespace paint {

paint_canvas::paint_canvas(paint_model &model) : model_{model}
{
}

void paint_canvas::do_set_size(terminalpp::extent const &size)
{
    munin::basic_component::do_set_size(size);
    model_.resize(
        {std::max(terminalpp::coordinate_type{0}, size.width_ - 2),
         std::max(terminalpp::coordinate_type{0}, size.height_ - 2)});
}

auto paint_canvas::do_get_preferred_size() const -> terminalpp::extent
{
    return {34, 18};
}

auto paint_canvas::do_can_receive_focus() const -> bool
{
    return false;
}

void paint_canvas::do_draw(
    munin::render_surface &surface, terminalpp::rectangle const &region) const
{
}

void paint_canvas::do_event(
    std::any const &event, munin::event_context &context)
{
    if (auto const *mouse = std::any_cast<munin::mouse_event>(&event);
        mouse != nullptr)
    {
        if (mouse->action_ == munin::mouse_event_type::button_down
            && mouse->button_ == terminalpp::mouse::button::left
            && is_inside_interior(mouse->position_))
        {
            is_stroking_ = true;
            paint_at(mouse->position_);
        }
        else if (mouse->action_ == munin::mouse_event_type::button_up)
        {
            is_stroking_ = false;
        }
    }
    else if (auto const *mouse =
                 std::any_cast<terminalpp::mouse::event>(&event);
             mouse != nullptr && is_stroking_ && mouse->is_motion_
             && is_inside_interior(mouse->position_))
    {
        paint_at(mouse->position_);
    }
}

auto paint_canvas::interior_position(terminalpp::point position) const
    -> terminalpp::point
{
    return {position.x_ - 1, position.y_ - 1};
}

auto paint_canvas::is_inside_interior(terminalpp::point position) const -> bool
{
    auto const size = get_size();
    return position.x_ > 0 && position.x_ < size.width_ - 1 && position.y_ > 0
        && position.y_ < size.height_ - 1;
}

void paint_canvas::paint_at(terminalpp::point position)
{
    model_.paint(interior_position(position));
    on_redraw({
        {position, {1, 1}}
    });
}

}  // namespace paint
