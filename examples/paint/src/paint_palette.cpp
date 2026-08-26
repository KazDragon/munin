#include "paint/paint_palette.hpp"

#include "paint/paint_model.hpp"

#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/element.hpp>
#include <terminalpp/graphics.hpp>

#include <any>
#include <array>

namespace paint {
namespace {

auto constexpr colours = std::array{
    terminalpp::graphics::colour::black,
    terminalpp::graphics::colour::red,
    terminalpp::graphics::colour::green,
    terminalpp::graphics::colour::yellow,
    terminalpp::graphics::colour::blue,
    terminalpp::graphics::colour::magenta,
    terminalpp::graphics::colour::cyan,
    terminalpp::graphics::colour::white};

[[nodiscard]] auto brush_at(terminalpp::point position) -> terminalpp::attribute
{
    auto result = terminalpp::attribute{};
    result.background_colour_ = colours[position.y_];

    if (position.x_ == 1)
    {
        result.intensity_ = terminalpp::graphics::intensity::bold;
    }

    return result;
}

[[nodiscard]] auto is_swatch_position(terminalpp::point position) -> bool
{
    return position.x_ >= 0 && position.x_ < 2 && position.y_ >= 0
        && position.y_ < std::ssize(colours);
}

[[nodiscard]] auto selection_marker_attribute(terminalpp::attribute brush)
    -> terminalpp::attribute
{
    brush.foreground_colour_ =
        brush.intensity_ == terminalpp::graphics::intensity::bold
            ? terminalpp::graphics::colour::black
            : terminalpp::graphics::colour::white;
    return brush;
}

}  // namespace

paint_palette::paint_palette(paint_model &model) : model_{model}
{
}

auto paint_palette::do_get_preferred_size() const -> terminalpp::extent
{
    return {2, 10};
}

auto paint_palette::do_can_receive_focus() const -> bool
{
    return false;
}

void paint_palette::do_draw(
    munin::render_surface &surface, terminalpp::rectangle const &region) const
{
    for (auto y = terminalpp::coordinate_type{0}; y < std::ssize(colours); ++y)
    {
        for (auto x = terminalpp::coordinate_type{0}; x < 2; ++x)
        {
            auto const brush = brush_at({x, y});
            auto const glyph = static_cast<terminalpp::byte>(
                brush == model_.selected_brush() ? 'O' : ' ');
            surface[x][y] =
                terminalpp::element{glyph, selection_marker_attribute(brush)};
        }
    }

    if (get_size().height_ > std::ssize(colours) + 1)
    {
        auto const y = terminalpp::coordinate_type{std::ssize(colours) + 1};
        for (auto x = terminalpp::coordinate_type{0}; x < get_size().width_;
             ++x)
        {
            surface[x][y] = terminalpp::element{' ', model_.selected_brush()};
        }
    }
}

void paint_palette::do_event(
    std::any const &event, munin::event_context &context)
{
    auto const *mouse = std::any_cast<munin::mouse_event>(&event);

    if (mouse != nullptr && mouse->action_ == munin::mouse_event_type::click
        && mouse->button_ == terminalpp::mouse::button::left
        && is_swatch_position(mouse->position_))
    {
        model_.select_brush(brush_at(mouse->position_));
        on_redraw({
            {{0, 0}, get_size()}
        });
    }
}

}  // namespace paint
