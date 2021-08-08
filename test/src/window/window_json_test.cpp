#include "window_test.hpp"
#include <munin/window.hpp>
#include <gtest/gtest.h>

TEST_F(a_window, reports_attributes_as_json)
{
    EXPECT_CALL(*content_, do_to_json())
        .WillOnce(
            []() -> nlohmann::json
            {
                return {
                    { "type", "mock_content" }
                };
            });

    nlohmann::json json = window_->to_json();
    ASSERT_EQ("window", json["type"]);

    nlohmann::json content_json = json["content"];
    ASSERT_EQ("mock_content", content_json["type"]);
}