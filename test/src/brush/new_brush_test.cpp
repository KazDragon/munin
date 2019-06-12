#include <munin/brush.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

TEST(a_new_brush, has_a_singular_preferred_size)
{
    munin::brush brush;
    ASSERT_EQ(terminalpp::extent(1, 1), brush.get_preferred_size());
}

TEST(a_new_brush_with_a_single_line_pattern, has_a_preferred_size_with_the_width_of_that_line)
{
    using namespace terminalpp::literals;
    auto const pattern = "abcde"_ts;
    munin::brush brush(pattern);

    ASSERT_EQ(terminalpp::extent(pattern.size(), 1), brush.get_preferred_size());
}

TEST(a_new_brush_with_a_multi_line_pattern, has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
    using namespace terminalpp::literals;
    auto const pattern = std::vector<terminalpp::string> {
        "abcde"_ts,
        "abcdefg"_ts
    };

    munin::brush brush(pattern);

    ASSERT_EQ(terminalpp::extent(pattern[1].size(), 2), brush.get_preferred_size());
}

TEST(a_new_brush, draws_whitespace_on_the_canvas)
{
    munin::brush brush;
    brush.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    brush.draw(surface, {{}, brush.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
}

TEST(a_new_brush_with_a_single_line_pattern, draws_that_pattern_repeatedly)
{
    using namespace terminalpp::literals;
    terminalpp::string pattern = "abc"_ts;

    munin::brush brush(pattern);
    brush.set_size({6, 2});

    terminalpp::canvas canvas({8, 4});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    surface.offset_by({1, 1});
    brush.draw(surface, {{}, brush.get_size()});

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[6][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[7][0]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[6][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[7][1]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[5][2]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[6][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[7][2]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[4][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[6][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[7][3]);
}

TEST(a_new_brush_with_a_multi_line_pattern, draws_that_pattern_repeatedly)
{
    using namespace terminalpp::literals;
    std::vector<terminalpp::string> pattern = {
        "ab"_ts,
        "cd"_ts
    };

    munin::brush brush(pattern);
    brush.set_size({4, 4});

    terminalpp::canvas canvas({6, 6});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    surface.offset_by({1, 1});
    brush.draw(surface, {{}, brush.get_size()});

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][0]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][1]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][2]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[1][3]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[2][3]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[3][3]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[4][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][3]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][4]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[1][4]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[2][4]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[3][4]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[4][4]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][4]);

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][5]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][5]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][5]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][5]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[4][5]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[5][5]);
}

TEST(make_brush_with_no_pattern, makes_a_new_default_brush)
{
    std::shared_ptr<munin::brush> brush = munin::make_brush();
    ASSERT_EQ(terminalpp::extent(1, 1), brush->get_preferred_size());
}

TEST(make_brush_with_a_string_pattern, makes_a_single_line_brush)
{
    using namespace terminalpp::literals;
    std::shared_ptr<munin::brush> brush = munin::make_brush("test"_ts);
    ASSERT_EQ(terminalpp::extent(4, 1), brush->get_preferred_size());
}

TEST(make_brush_with_a_vector_pattern, makes_a_multi_line_brush)
{
    std::vector<terminalpp::string> pattern = {
        "ab",
        "cd"
    };

    std::shared_ptr<munin::brush> brush = munin::make_brush(pattern);
    ASSERT_EQ(terminalpp::extent(2, 2), brush->get_preferred_size());
}
