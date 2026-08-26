#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <paint/paint_canvas.hpp>
#include <paint/paint_model.hpp>
#include <terminalpp/graphics.hpp>
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
