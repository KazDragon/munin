#include <gtest/gtest.h>
#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <paint/paint_model.hpp>
#include <paint/paint_palette.hpp>
#include <terminalpp/graphics.hpp>

namespace {

[[nodiscard]] auto normal_red_brush() -> terminalpp::attribute
{
    terminalpp::attribute result;
    result.background_colour_ = terminalpp::graphics::colour::red;
    return result;
}

[[nodiscard]] auto bright_red_brush() -> terminalpp::attribute
{
    auto result = normal_red_brush();
    result.intensity_ = terminalpp::graphics::intensity::bold;
    return result;
}

[[nodiscard]] auto bright_white_brush() -> terminalpp::attribute
{
    terminalpp::attribute result;
    result.background_colour_ = terminalpp::graphics::colour::white;
    result.intensity_ = terminalpp::graphics::intensity::bold;
    return result;
}

}  // namespace

TEST(a_paint_palette, ignores_button_down_but_selects_on_synthesized_click)
{
    paint::paint_model model({1, 1});
    auto palette = paint::paint_palette{model};
    auto context = munin::event_context{};

    palette.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {0, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(bright_white_brush(), model.selected_brush());

    palette.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::click,
            .position_ = {0, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(normal_red_brush(), model.selected_brush());
}

TEST(a_paint_palette, maps_the_right_column_to_bright_ansi_colours)
{
    paint::paint_model model({1, 1});
    auto palette = paint::paint_palette{model};
    auto context = munin::event_context{};

    palette.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::click,
            .position_ = {1, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(bright_red_brush(), model.selected_brush());
}
