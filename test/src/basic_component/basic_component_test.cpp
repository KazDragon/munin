#include "fake_basic_component.hpp"

#include <gtest/gtest.h>
#include <munin/basic_component.hpp>

TEST(the_size_of_a_basic_component, can_be_altered)
{
    fake_basic_component component;

    terminalpp::extent const expected_size = {10, 15};

    component.set_size(expected_size);

    ASSERT_EQ(expected_size, component.get_size());
}

TEST(the_position_of_a_basic_component, can_be_altered)
{
    fake_basic_component component;

    terminalpp::point const expected_position = {32, 42};

    component.set_position(expected_position);

    ASSERT_EQ(expected_position, component.get_position());
}

TEST(a_basic_component, reports_attributes_as_json)
{
    fake_basic_component basic;
    munin::component &component = basic;

    terminalpp::point position{57, 50};
    terminalpp::extent size{3, 4};

    component.set_position(position);
    component.set_size(size);

    nlohmann::json json = component.to_json();
    ASSERT_EQ("basic_component", json["type"]);
    ASSERT_EQ(position.x_, json["position"]["x"]);
    ASSERT_EQ(position.y_, json["position"]["y"]);
    ASSERT_EQ(size.width_, json["size"]["width"]);
    ASSERT_EQ(size.height_, json["size"]["height"]);
    ASSERT_EQ(0, json["preferred_size"]["width"]);
    ASSERT_EQ(0, json["preferred_size"]["height"]);
    ASSERT_EQ(false, json["has_focus"]);
    ASSERT_EQ(false, json["cursor_state"]);
    ASSERT_EQ(0, json["cursor_position"]["x"]);
    ASSERT_EQ(0, json["cursor_position"]["y"]);
}
