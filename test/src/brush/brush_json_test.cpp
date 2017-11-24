#include <munin/brush.hpp>
#include <gtest/gtest.h>

TEST(a_default_brush, reports_attributes_as_json)
{
    munin::brush brush;

    nlohmann::json json = brush.to_json();
    ASSERT_EQ("brush", json["type"]);
    ASSERT_EQ(1,       json["pattern"]["size"]);
    ASSERT_EQ(" ",     json["pattern"]["content"][0]);
}

TEST(a_single_line_brush, reports_attributes_as_json)
{
    munin::brush brush("test");

    nlohmann::json json = brush.to_json();
    ASSERT_EQ("brush", json["type"]);
    ASSERT_EQ(1,       json["pattern"]["size"]);
    ASSERT_EQ("test",  json["pattern"]["content"][0]);
}

TEST(a_multi_line_brush, reports_attributes_as_json)
{
    munin::brush brush(std::vector<terminalpp::string>{
        "test",
        "success"
    });

    nlohmann::json json = brush.to_json();
    ASSERT_EQ("brush",   json["type"]);
    ASSERT_EQ(2,         json["pattern"]["size"]);
    ASSERT_EQ("test",    json["pattern"]["content"][0]);
    ASSERT_EQ("success", json["pattern"]["content"][1]);
}

