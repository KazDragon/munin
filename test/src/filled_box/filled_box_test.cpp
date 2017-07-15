#include <munin/filled_box.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>

TEST(make_fill, make_a_new_filled_box)
{
    std::shared_ptr<munin::filled_box> filled_box = munin::make_fill('F');
}

TEST(a_filled_box, can_have_its_preferred_size_set)
{
    int called = 0;
    auto on_preferred_size_changed = [&called]
    {
        ++called;
    };
    
    munin::filled_box filled_box;
    filled_box.on_preferred_size_changed.connect(on_preferred_size_changed);
    
    filled_box.set_preferred_size({2, 2});
    
    ASSERT_EQ(terminalpp::extent(2, 2), filled_box.get_preferred_size());
    ASSERT_EQ(1, called);
}

TEST(a_filled_box, can_have_its_fill_set)
{
    std::vector<munin::rectangle> redraw_regions;
    int called = 0;
    auto on_redraw = [&called, &redraw_regions](auto const &regions)
    {
        redraw_regions = regions;
        ++called;
    };
    
    munin::filled_box filled_box;
    filled_box.on_redraw.connect(on_redraw);
    
    filled_box.set_fill(terminalpp::element{'X'});
    
    ASSERT_EQ(terminalpp::element{'X'}, filled_box.get_fill());
    ASSERT_EQ(1, called);
    ASSERT_EQ(1u, redraw_regions.size());
    ASSERT_EQ(munin::rectangle({}, filled_box.get_size()), redraw_regions[0]);
}

TEST(a_filled_box, draws_its_fill)
{
    munin::filled_box filled_box;
    filled_box.set_fill('Y');
    filled_box.set_size({1, 1});
    
    terminalpp::canvas canvas({2, 2});
    terminalpp::canvas_view canvas_view(canvas);
    munin::context context(canvas_view);
    filled_box.draw(context, {{}, {1, 1}});
    
    ASSERT_EQ(terminalpp::element('Y'), canvas[0][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][1]);
}

TEST(a_filled_box, draws_only_within_given_region)
{
    munin::filled_box filled_box;
    filled_box.set_fill('Y');
    filled_box.set_size({2, 2});
    
    terminalpp::canvas canvas({3, 4});
    terminalpp::canvas_view canvas_view(canvas);
    canvas_view.offset_by({1, 1});
    munin::context context(canvas_view);
    
    filled_box.draw(context, {{}, {1, 2}});
    
    ASSERT_EQ(terminalpp::element(' '), canvas[0][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][1]);
    ASSERT_EQ(terminalpp::element('Y'), canvas[1][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][2]);
    ASSERT_EQ(terminalpp::element('Y'), canvas[1][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][3]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][3]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][3]);
}
