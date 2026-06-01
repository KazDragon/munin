#include <gtest/gtest.h>
#include <munin/button.hpp>

TEST(a_button, reports_attributes_as_json)
{
    munin::button button("  OK  ");
    button.set_id("ok_button");
    nlohmann::json json = button.to_json();

    ASSERT_EQ("button", json["type"]);
    ASSERT_EQ("ok_button", json["id"]);
    ASSERT_EQ("OK", json["name"]);
    ASSERT_EQ(json.end(), json.find("subcomponents"));
}

TEST(a_button, reports_its_visible_text_as_accessible_name)
{
    munin::button button("  Cancel  ");

    nlohmann::json json = button.to_json();

    ASSERT_EQ("Cancel", json["name"]);
}

TEST(a_button, reports_its_current_bounds_as_json)
{
    munin::button button("OK");
    button.set_position({3, 5});
    button.set_size({7, 2});

    nlohmann::json json = button.to_json();

    ASSERT_EQ(3, json["position"]["x"]);
    ASSERT_EQ(5, json["position"]["y"]);
    ASSERT_EQ(7, json["size"]["width"]);
    ASSERT_EQ(2, json["size"]["height"]);
}

TEST(a_button, reports_its_preferred_size_as_json)
{
    munin::button button("OK");

    nlohmann::json json = button.to_json();

    ASSERT_EQ(4, json["preferred_size"]["width"]);
    ASSERT_EQ(3, json["preferred_size"]["height"]);
}

TEST(a_button, reports_when_it_has_focus_as_json)
{
    munin::button button("OK");
    button.set_focus();

    nlohmann::json json = button.to_json();

    ASSERT_EQ(true, json["has_focus"]);
}

TEST(a_button, reports_when_it_does_not_have_focus_as_json)
{
    munin::button button("OK");

    nlohmann::json json = button.to_json();

    ASSERT_EQ(false, json["has_focus"]);
}

TEST(a_button, reports_its_cursor_state_as_json)
{
    munin::button button("OK");

    nlohmann::json json = button.to_json();

    ASSERT_EQ(false, json["cursor_state"]);
    ASSERT_EQ(0, json["cursor_position"]["x"]);
    ASSERT_EQ(0, json["cursor_position"]["y"]);
}
