#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>

using a_new_text_area = a_text_area;

TEST_F(a_new_text_area, has_zero_cursor_position)
{
    ASSERT_EQ(terminalpp::point(0, 0), text_area_.get_cursor_position());
}

TEST_F(a_new_text_area, has_cursor_enabled)
{
    ASSERT_TRUE(text_area_.get_cursor_state());
}

TEST_F(a_new_text_area, has_caret_position_zero)
{
    ASSERT_EQ(0, text_area_.get_caret_position());
}

TEST_F(a_new_text_area, has_length_zero)
{
    ASSERT_EQ(0, text_area_.get_length());
}

TEST_F(a_new_text_area, has_a_preferred_width_of_zero_and_a_preferred_size_of_one)
{
    // A text area always wants to put its cursor somewhere, and will use a
    // textel of height 1 if it can.
    ASSERT_EQ(terminalpp::extent(0, 1), text_area_.get_preferred_size());
}

TEST_F(a_new_text_area, draws_only_spaces)
{
    text_area_.set_size({2, 2});

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
    text_area_.draw(surface, {{}, text_area_.get_size()});

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