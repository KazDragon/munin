#include <gtest/gtest.h>
#include <munin/button.hpp>

TEST(a_button, reports_attributes_as_json)
{
    munin::button button("  OK  ");
    nlohmann::json json = button.to_json();

    ASSERT_EQ("button", json["type"]);
}
