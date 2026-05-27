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
