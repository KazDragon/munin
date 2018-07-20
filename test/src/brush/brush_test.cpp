#include <munin/brush.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <gtest/gtest.h>

TEST(a_brush_with_its_pattern_set_empty, draws_whitespace_on_the_canvas)
{
    using namespace terminalpp::literals;
    munin::brush brush("abc"_ts);

    int called = 0;
    auto on_preferred_size_changed = [&called]
    {
        ++called;
    };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    brush.set_pattern();
    
    ASSERT_EQ(1, called);
    
    brush.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});

    for (terminalpp::coordinate_type row = 0;
         row < canvas.size().height;
         ++row)
    {
        for (terminalpp::coordinate_type col = 0;
             col < canvas.size().width;
             ++col)
        {
            canvas[col][row] = 'X';
        }
    }

    munin::render_surface cv{canvas};
    brush.draw(cv, {{}, brush.get_size()});

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

TEST(a_brush_with_its_pattern_set_to_a_single_line_pattern, draws_that_pattern_repeatedly)
{
    using namespace terminalpp::literals;
    
    munin::brush brush;

    int called = 0;
    auto on_preferred_size_changed = [&called]
    {
        ++called;
    };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    terminalpp::string const pattern = "abc"_ts;
    brush.set_pattern(pattern);
    
    ASSERT_EQ(1, called);

    brush.set_size({6, 2});

    terminalpp::canvas canvas({8, 4});

    for (terminalpp::coordinate_type row = 0;
         row < canvas.size().height;
         ++row)
    {
        for (terminalpp::coordinate_type col = 0;
             col < canvas.size().width;
             ++col)
        {
            canvas[col][row] = 'X';
        }
    }

    munin::render_surface cv{canvas};
    cv.offset_by({1, 1});
    brush.draw(cv, {{}, brush.get_size()});

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

TEST(a_brush_with_its_pattern_set_to_a_multi_line_pattern, draws_that_pattern_repeatedly)
{
    using namespace terminalpp::literals;
    
    munin::brush brush;

    int called = 0;
    auto on_preferred_size_changed = [&called]
    {
        ++called;
    };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    std::vector<terminalpp::string> const pattern = {
        "ab"_ts,
        "cd"_ts
    };

    brush.set_pattern(pattern);
    
    ASSERT_EQ(1, called);

    brush.set_size({4, 4});

    terminalpp::canvas canvas({6, 6});

    for (terminalpp::coordinate_type row = 0;
         row < canvas.size().height;
         ++row)
    {
        for (terminalpp::coordinate_type col = 0;
             col < canvas.size().width;
             ++col)
        {
            canvas[col][row] = 'X';
        }
    }

    munin::render_surface cv{canvas};
    cv.offset_by({1, 1});
    brush.draw(cv, {{}, brush.get_size()});

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
