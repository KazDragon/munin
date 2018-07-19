#include <munin/image.hpp>
#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

TEST(an_image_with_its_content_set_empty, draws_fill_on_the_canvas)
{
    using namespace terminalpp::literals;
    munin::image image("test"_ts, ' ');

    image.set_content();

    image.set_size({6,3});
    terminalpp::canvas canvas({6, 3});

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

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][2]);
}

TEST(an_image_with_its_content_set_to_single_line, draws_line_on_the_canvas)
{
    using namespace terminalpp::literals;
    munin::image image(' ');
    image.set_size({6,3});

    image.set_content("test"_ts);

    terminalpp::canvas canvas({6, 3});

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

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'s'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][2]);
}

TEST(an_image_with_its_content_set_to_multi_line, draws_lines_on_the_canvas)
{
    using namespace terminalpp::literals;
    munin::image image(' ');
    image.set_size({6,4});

    std::vector<terminalpp::string> const content = {
        "test"_ts,
        "abcd"_ts
    };

    image.set_content(content);

    terminalpp::canvas canvas({6, 4});

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

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'s'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][3]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][3]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][3]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][3]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][3]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][3]);
}

TEST(an_image, sets_its_content_empty_when_set_to_an_empty_string)
{
    munin::image image("test");
    image.set_size({4, 1});

    image.set_content("");

    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());

    terminalpp::canvas canvas({4, 1});

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

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][0]);
}

TEST(an_image, can_have_its_fill_set)
{
    munin::image image("test");
    image.set_size({6, 3});

    image.set_fill('!');

    terminalpp::canvas canvas({6, 3});

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

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    ASSERT_EQ(terminalpp::element{'!'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'s'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{'!'}, canvas[5][2]);
}
