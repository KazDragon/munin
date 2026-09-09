#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <munin/render_surface.hpp>
#include <paint/paint_canvas.hpp>
#include <paint/paint_model.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/graphics.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/rectangle.hpp>

#include <vector>

using testing::ElementsAre;

namespace {

[[nodiscard]] auto bright_white_brush() -> terminalpp::attribute
{
    terminalpp::attribute result;
    result.background_colour_ = terminalpp::graphics::colour::white;
    result.intensity_ = terminalpp::graphics::intensity::bold;
    return result;
}

}  // namespace

TEST(a_paint_canvas, paints_interior_left_button_downs_and_redraws_that_cell)
{
    paint::paint_model model({3, 2});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({5, 4});
    auto context = munin::event_context{};
    auto redraw_regions = std::vector<terminalpp::rectangle>{};
    canvas.on_redraw.connect(
        [&redraw_regions](auto const &regions) { redraw_regions = regions; });

    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {2, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(bright_white_brush(), model.at({1, 0}));
    EXPECT_THAT(
        redraw_regions,
        ElementsAre(terminalpp::rectangle{
            {2, 1},
            {1, 1}
    }));
}

TEST(a_paint_canvas, can_accept_focus)
{
    paint::paint_model model({1, 1});
    auto canvas = paint::paint_canvas{model};
    auto focus_set_count = 0;
    canvas.on_focus_set.connect([&] { ++focus_set_count; });

    canvas.set_focus();

    EXPECT_TRUE(canvas.has_focus());
    EXPECT_EQ(1, focus_set_count);
}

TEST(a_paint_canvas, paints_reported_motion_positions_during_a_left_stroke)
{
    paint::paint_model model({4, 3});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({6, 5});
    auto context = munin::event_context{};

    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {1, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);
    canvas.event(
        terminalpp::mouse::event{
            .action_ = terminalpp::mouse::event_type::no_button_change,
            .position_ = {3, 2},
            .button_ = terminalpp::mouse::button::left,
            .is_motion_ = true
    },
        context);
    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_up,
            .position_ = {3, 2},
            .button_ = terminalpp::mouse::button::left
    },
        context);
    canvas.event(
        terminalpp::mouse::event{
            .action_ = terminalpp::mouse::event_type::no_button_change,
            .position_ = {4, 3},
            .button_ = terminalpp::mouse::button::left,
            .is_motion_ = true
    },
        context);

    EXPECT_EQ(bright_white_brush(), model.at({0, 0}));
    EXPECT_EQ(bright_white_brush(), model.at({2, 1}));
    EXPECT_NE(bright_white_brush(), model.at({3, 2}));
}

TEST(a_paint_canvas, ignores_right_and_middle_button_downs)
{
    paint::paint_model model({3, 3});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({5, 5});
    auto context = munin::event_context{};

    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {1, 1},
            .button_ = terminalpp::mouse::button::right
    },
        context);
    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {2, 2},
            .button_ = terminalpp::mouse::button::middle
    },
        context);

    EXPECT_NE(bright_white_brush(), model.at({0, 0}));
    EXPECT_NE(bright_white_brush(), model.at({1, 1}));
}

TEST(a_paint_canvas, treats_in_and_out_events_as_no_ops_during_a_stroke)
{
    paint::paint_model model({4, 3});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({6, 5});
    auto context = munin::event_context{};

    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {1, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);
    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::out,
            .position_ = {5, 4},
            .button_ = terminalpp::mouse::button::left
    },
        context);
    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::in,
            .position_ = {2, 2},
            .button_ = terminalpp::mouse::button::left
    },
        context);
    canvas.event(
        terminalpp::mouse::event{
            .action_ = terminalpp::mouse::event_type::no_button_change,
            .position_ = {2, 2},
            .button_ = terminalpp::mouse::button::left,
            .is_motion_ = true
    },
        context);

    EXPECT_EQ(bright_white_brush(), model.at({0, 0}));
    EXPECT_EQ(bright_white_brush(), model.at({1, 1}));
    EXPECT_NE(bright_white_brush(), model.at({3, 2}));
}

TEST(a_paint_canvas, resizes_the_model_to_match_the_framed_interior)
{
    paint::paint_model model({1, 1});
    auto canvas = paint::paint_canvas{model};
    auto context = munin::event_context{};

    canvas.set_size({5, 4});
    canvas.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {2, 2},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(bright_white_brush(), model.at({1, 1}));
}

TEST(a_paint_canvas, draws_painted_cells_as_spaces_with_cell_attributes)
{
    paint::paint_model model({1, 1});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({3, 3});
    model.paint({0, 0});
    auto output = terminalpp::canvas{
        {3, 3}
    };
    auto surface = munin::render_surface{output};

    canvas.draw(surface);

    EXPECT_EQ(terminalpp::element(' ', bright_white_brush()), output[1][1]);
}

TEST(a_paint_canvas, draws_an_ascii_frame_around_the_interior)
{
    paint::paint_model model({1, 1});
    auto canvas = paint::paint_canvas{model};
    canvas.set_size({3, 3});
    auto output = terminalpp::canvas{
        {3, 3}
    };
    auto surface = munin::render_surface{output};

    canvas.draw(surface);

    EXPECT_EQ('+', output[0][0].glyph_.character_);
    EXPECT_EQ('-', output[1][0].glyph_.character_);
    EXPECT_EQ('|', output[0][1].glyph_.character_);
    EXPECT_EQ('+', output[2][2].glyph_.character_);
}
