#include <munin/image.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
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

    terminalpp::canvas_view cv{canvas};
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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

    terminalpp::canvas_view cv{canvas};
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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

    terminalpp::canvas_view cv{canvas};
    munin::context ctx(cv);
    image.draw(ctx, {{}, image.get_size()});

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

TEST(an_image_with_empty_content, redraws_nothing_when_setting_content_to_empty)
{
    munin::image image;
    image.set_size({5, 3});
    
    int preferred_size_changed_called = 0;
    auto on_preferred_size_changed = [&preferred_size_changed_called]
    {
        ++preferred_size_changed_called;
    };
    image.on_preferred_size_changed.connect(on_preferred_size_changed);
    
    int redraw_called = 0;
    std::vector<munin::rectangle> redraw_regions;
    auto on_redraw = [&redraw_called, &redraw_regions](auto const &regions)
    {
        ++redraw_called;
        redraw_regions = regions;
    };
    image.on_redraw.connect(on_redraw);

    image.set_content();

    ASSERT_EQ(0, preferred_size_changed_called);
    ASSERT_EQ(0, redraw_called);
    ASSERT_TRUE(redraw_regions.empty());
}

TEST(an_image_with_empty_content, redraws_new_image_area_when_setting_content_to_single_line)
{
    using namespace terminalpp::literals;
    munin::image image;
    image.set_size({6, 3});

    int preferred_size_changed_called = 0;
    terminalpp::extent preferred_size;
    auto on_preferred_size_changed = 
        [&preferred_size_changed_called, &preferred_size, &image]
    {
        ++preferred_size_changed_called;
        preferred_size = image.get_preferred_size();
    };
    image.on_preferred_size_changed.connect(on_preferred_size_changed);
   
    int redraw_called = 0;
    std::vector<munin::rectangle> redraw_regions;
    auto on_redraw = [&redraw_called, &redraw_regions](auto const &regions)
    {
        ++redraw_called;
        redraw_regions = regions;
    };

    image.on_redraw.connect(on_redraw);
    
    image.set_content("test"_ts);
    
    ASSERT_EQ(1, preferred_size_changed_called);
    ASSERT_EQ(terminalpp::extent(4, 1), preferred_size);

    ASSERT_EQ(1, redraw_called);
    ASSERT_EQ(1u, redraw_regions.size());
    ASSERT_EQ(munin::rectangle({1, 1}, {4, 1}), redraw_regions[0]);
}
