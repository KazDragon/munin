#include "window_test.hpp"
#include <munin/window.hpp>
#include <gtest/gtest.h>

using testing::Invoke;

TEST_F(a_window, reports_attributes_as_json)
{
    window_->set_size({69, 37});
    
    EXPECT_CALL(*content_, do_to_json())
        .WillOnce(Invoke([]() -> nlohmann::json
        {
            return {
                { "type", "mock_content" }
            };
        }));

    nlohmann::json json = window_->to_json();
    ASSERT_EQ("window", json["type"]);
    ASSERT_EQ(69,       json["size"]["width"]);
    ASSERT_EQ(37,       json["size"]["height"]);
    
    nlohmann::json content_json = json["content"];
    ASSERT_EQ("mock_content", content_json["type"]);
}