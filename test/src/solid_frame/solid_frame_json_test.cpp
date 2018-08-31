#include <munin/solid_frame.hpp>
#include <gtest/gtest.h>

TEST(a_solid_frame, reports_attributes_as_json)
{
    static auto const position = terminalpp::point{4, 7};
    static auto const size = terminalpp::extent{3, 2};
    
    munin::solid_frame frame;
    frame.set_position(position);
    frame.set_size(size);
    
    nlohmann::json json = frame.to_json();
    //ASSERT_EQ("solid_frame", json["type"]);
    /*
    ASSERT_EQ(position.x,    json["position"]["x"]);
    ASSERT_EQ(position.y,    json["position"]["y"]);
    ASSERT_EQ(size.width,    json["size"]["width"]);
    ASSERT_EQ(size.height,   json["size"]["height"]);
    ASSERT_EQ(0,             json["preferred_size"]["width"]);
    ASSERT_EQ(0,             json["preferred_size"]["height"]);
    ASSERT_EQ(false,         json["has_focus"]);
    ASSERT_EQ(false,         json["cursor_state"]);
    ASSERT_EQ(0,             json["cursor_position"]["x"]);
    ASSERT_EQ(0,             json["cursor_position"]["y"]);
    */
}