#include <munin/toggle_button.hpp>
#include <gtest/gtest.h>

TEST(a_toggle_button, reports_attributes_as_json)
{
    munin::toggle_button toggle_button;
    nlohmann::json json = toggle_button.to_json();

    ASSERT_EQ("toggle_button", json["type"]);
    ASSERT_EQ(false, json["state"]);

    toggle_button.set_toggle_state(true);
    json = toggle_button.to_json();

    ASSERT_EQ("toggle_button", json["type"]);
    ASSERT_EQ(true, json["state"]);

    std::cout << json;
}
