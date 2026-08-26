#include <gtest/gtest.h>
#include <paint/paint_model.hpp>
#include <terminalpp/attribute.hpp>
#include <terminalpp/graphics.hpp>

namespace {

[[nodiscard]] auto black_cell() -> terminalpp::attribute
{
    terminalpp::attribute result;
    result.background_colour_ = terminalpp::graphics::colour::black;
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

TEST(a_paint_model, starts_with_black_cells_and_a_bright_white_brush)
{
    paint::paint_model model({2, 3});

    EXPECT_EQ(black_cell(), model.at({0, 0}));
    EXPECT_EQ(black_cell(), model.at({1, 2}));
    EXPECT_EQ(bright_white_brush(), model.selected_brush());
}
