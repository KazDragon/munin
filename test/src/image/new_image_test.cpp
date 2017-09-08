#include <munin/image.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>

TEST(a_new_image, has_a_zero_preferred_size)
{
    munin::image image;
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(a_new_image_with_a_fill, has_a_zero_preferred_size)
{
    munin::image image(terminalpp::element('X'));
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(a_new_image_with_single_line_content, has_a_preferred_size_with_the_width_of_that_line)
{
    using namespace terminalpp::literals;
    auto const content = "abcde"_ts;
    munin::image image(content);
    
    ASSERT_EQ(terminalpp::extent(content.size(), 1), image.get_preferred_size());
}

TEST(a_new_image_with_single_line_content_and_a_fill, has_a_preferred_size_with_the_width_of_that_line)
{
    using namespace terminalpp::literals;
    auto const content = "abcde"_ts;
    munin::image image(content, terminalpp::element('Y'));
    
    ASSERT_EQ(terminalpp::extent(content.size(), 1), image.get_preferred_size());
}

TEST(a_new_image_with_multi_line_content, has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
    using namespace terminalpp::literals;
    auto const content = std::vector<terminalpp::string> {
        "abcde"_ts,
        "abcdefg"_ts
    };
    
    munin::image image(content);
    
    ASSERT_EQ(terminalpp::extent(content[1].size(), 2), image.get_preferred_size());
}

TEST(a_new_image_with_multi_line_content_and_a_fill, has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
    using namespace terminalpp::literals;
    auto const content = std::vector<terminalpp::string> {
        "abcde"_ts,
        "abcdefg"_ts
    };
    
    munin::image image(content, 'Z');
    
    ASSERT_EQ(terminalpp::extent(content[1].size(), 2), image.get_preferred_size());
}

/*
TEST(a_new_image, draws_whitespace_on_the_canvas)
{
    munin::image image;
    image.set_size({2, 2});

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

    terminalpp::canvas_view cv{canvas};
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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

TEST(a_new_image_with_a_single_line_content, draws_that_content_repeatedly)
{
    using namespace terminalpp::literals;
    terminalpp::string content = "abc"_ts;
    
    munin::image image(content);
    image.set_size({6, 2});

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

    terminalpp::canvas_view cv{canvas};
    cv.offset_by({1, 1});
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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

TEST(a_new_image_with_a_multi_line_content, draws_that_content_repeatedly)
{
    using namespace terminalpp::literals;
    std::vector<terminalpp::string> content = {
        "ab"_ts,
        "cd"_ts
    };
    
    munin::image image(content);
    image.set_size({4, 4});

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

    terminalpp::canvas_view cv{canvas};
    cv.offset_by({1, 1});
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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
*/