#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

TEST(a_new_filled_box, has_a_singular_preferred_size)
{
    munin::filled_box filled_box;
    ASSERT_EQ(terminalpp::extent(1, 1), filled_box.get_preferred_size());
}

TEST(a_new_filled_box, draws_whitespace_on_the_canvas)
{
    munin::filled_box filled_box;
    filled_box.set_size({2, 2});

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
    filled_box.draw(surface, {{}, filled_box.get_size()});

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

TEST(a_new_filled_box, refuses_focus)
{
    munin::filled_box filled_box;
    filled_box.set_focus();
    ASSERT_FALSE(filled_box.has_focus());
}
