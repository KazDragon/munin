#include <munin/filled_box.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>

TEST(a_new_filled_box, has_a_whitespace_fill)
{
    munin::filled_box filled_box;
    ASSERT_EQ(terminalpp::element(' '), filled_box.get_fill());
}

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
    
    for (terminalpp::u32 row = 0; row < canvas.size().height; ++row)
    {
        for (terminalpp::u32 col = 0; col < canvas.size().width; ++col)
        {
            canvas[col][row] = 'X';
        }
    }
    
    terminalpp::canvas_view cv{canvas};
    munin::context ctx(cv);
    filled_box.draw(ctx, {{}, filled_box.get_size()});
    
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