#include "paint/paint_canvas.hpp"

#include "paint/paint_model.hpp"

#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/element.hpp>

#include <algorithm>
#include <any>

namespace paint {
namespace {

auto constexpr frame_thickness = terminalpp::coordinate_type{1};
auto constexpr frame_size = terminalpp::coordinate_type{2};

}  // namespace

paint_canvas::paint_canvas(paint_model &model) : model_{model}
{
}

void paint_canvas::do_set_size(terminalpp::extent const &size)
{
    munin::basic_component::do_set_size(size);
    model_.resize(
        {std::max(terminalpp::coordinate_type{0}, size.width_ - frame_size),
         std::max(terminalpp::coordinate_type{0}, size.height_ - frame_size)});
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
    auto const size = get_size();

    if (size.width_ > 0 && size.height_ > 0)
    {
        auto const east = size.width_ - 1;
        auto const south = size.height_ - 1;

        for (auto x = terminalpp::coordinate_type{0}; x < size.width_; ++x)
        {
            surface[x][0] = terminalpp::element{'-'};
            surface[x][south] = terminalpp::element{'-'};
        }

        for (auto y = terminalpp::coordinate_type{0}; y < size.height_; ++y)
        {
            surface[0][y] = terminalpp::element{'|'};
            surface[east][y] = terminalpp::element{'|'};
        }

        surface[0][0] = terminalpp::element{'+'};
        surface[east][0] = terminalpp::element{'+'};
        surface[0][south] = terminalpp::element{'+'};
        surface[east][south] = terminalpp::element{'+'};
    }

    for (auto y = frame_thickness; y < size.height_ - frame_thickness; ++y)
    {
        for (auto x = frame_thickness; x < size.width_ - frame_thickness; ++x)
        {
            auto const canvas_position = terminalpp::point{x, y};
            surface[x][y] = terminalpp::element{
                ' ', model_.at(interior_position(canvas_position))};
        }
    }
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
    return {position.x_ - frame_thickness, position.y_ - frame_thickness};
}

auto paint_canvas::is_inside_interior(terminalpp::point position) const -> bool
{
    auto const size = get_size();
    return position.x_ >= frame_thickness
        && position.x_ < size.width_ - frame_thickness
        && position.y_ >= frame_thickness
        && position.y_ < size.height_ - frame_thickness;
}

void paint_canvas::paint_at(terminalpp::point position)
{
    model_.paint(interior_position(position));
    on_redraw({
        {position, {1, 1}}
    });
}

}  // namespace paint
