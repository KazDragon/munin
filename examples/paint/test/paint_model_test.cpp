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

[[nodiscard]] auto red_brush() -> terminalpp::attribute
{
    terminalpp::attribute result;
    result.background_colour_ = terminalpp::graphics::colour::red;
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

TEST(a_paint_model, paints_an_in_bounds_cell_with_the_selected_brush)
{
    paint::paint_model model({2, 3});

    model.paint({1, 2});

    EXPECT_EQ(bright_white_brush(), model.at({1, 2}));
    EXPECT_EQ(black_cell(), model.at({0, 2}));
}

TEST(a_paint_model, ignores_out_of_bounds_paint_points)
{
    paint::paint_model model({2, 3});

    model.paint({2, 0});
    model.paint({0, 3});
    model.paint({-1, 0});
    model.paint({0, -1});

    EXPECT_EQ(black_cell(), model.at({0, 0}));
    EXPECT_EQ(black_cell(), model.at({1, 2}));
}

TEST(a_paint_model, preserves_overlap_and_fills_new_cells_black_when_resized)
{
    paint::paint_model model({2, 2});
    model.paint({1, 1});

    model.resize({3, 3});

    EXPECT_EQ(bright_white_brush(), model.at({1, 1}));
    EXPECT_EQ(black_cell(), model.at({2, 2}));
}

TEST(a_paint_model, accepts_zero_sized_resizes)
{
    paint::paint_model model({2, 2});
    model.paint({1, 1});

    model.resize({0, 0});
    model.resize({2, 2});

    EXPECT_EQ(black_cell(), model.at({1, 1}));
}

TEST(a_paint_model, paints_with_the_updated_selected_brush)
{
    paint::paint_model model({2, 2});

    model.select_brush(red_brush());
    model.paint({1, 1});

    EXPECT_EQ(red_brush(), model.selected_brush());
    EXPECT_EQ(red_brush(), model.at({1, 1}));
}
