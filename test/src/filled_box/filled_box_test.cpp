#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
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
    munin::render_surface surface{canvas};
    
    filled_box.draw(surface, {{}, {1, 1}});

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
    munin::render_surface surface{canvas};
    surface.offset_by({1, 1});

    filled_box.draw(surface, {{}, {1, 2}});

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

TEST(a_filled_box, reports_attributes_as_json)
{
    munin::filled_box box;
    munin::component &component = box;

    terminalpp::point position{37, 20};
    terminalpp::extent size{4, 5};
    terminalpp::extent preferred_size{7, 8};

    box.set_preferred_size(preferred_size);
    component.set_position(position);
    component.set_size(size);

    nlohmann::json json = component.to_json();
    ASSERT_EQ("filled_box",          json["type"]);
    ASSERT_EQ(position.x,            json["position"]["x"]);
    ASSERT_EQ(position.y,            json["position"]["y"]);
    ASSERT_EQ(size.width,            json["size"]["width"]);
    ASSERT_EQ(size.height,           json["size"]["height"]);
    ASSERT_EQ(preferred_size.width,  json["preferred_size"]["width"]);
    ASSERT_EQ(preferred_size.height, json["preferred_size"]["height"]);
    ASSERT_EQ(false,                 json["has_focus"]);
    ASSERT_EQ(false,                 json["cursor_state"]);
    ASSERT_EQ(0,                     json["cursor_position"]["x"]);
    ASSERT_EQ(0,                     json["cursor_position"]["y"]);
}
