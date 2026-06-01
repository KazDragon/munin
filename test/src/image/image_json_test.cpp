#include <gtest/gtest.h>
#include <munin/image.hpp>

TEST(a_default_image, reports_attributes_as_json)
{
    munin::image image;

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(0, json["content"]["size"]);
    ASSERT_EQ(" ", json["fill"]);
    ASSERT_EQ(json.end(), json.find("name"));
}

TEST(a_default_image_with_fill, reports_attributes_as_json)
{
    munin::image image('X');

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(0, json["content"]["size"]);
    ASSERT_EQ("X", json["fill"]);
}

TEST(a_single_line_image, reports_attributes_as_json)
{
    munin::image image("test");

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(1, json["content"]["size"]);
    ASSERT_EQ("test", json["content"]["content"][0]);
    ASSERT_EQ("test", json["name"]);
    ASSERT_EQ(" ", json["fill"]);
}

TEST(a_single_line_image_with_fill, reports_attributes_as_json)
{
    munin::image image("test", 'Z');

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(1, json["content"]["size"]);
    ASSERT_EQ("test", json["content"]["content"][0]);
    ASSERT_EQ("Z", json["fill"]);
}

TEST(a_multi_line_image, reports_attributes_as_json)
{
    munin::image image(std::vector<terminalpp::string>{"test", "success"});

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(2, json["content"]["size"]);
    ASSERT_EQ("test", json["content"]["content"][0]);
    ASSERT_EQ("success", json["content"]["content"][1]);
    ASSERT_EQ("test\nsuccess", json["name"]);
    ASSERT_EQ(" ", json["fill"]);
}

TEST(a_multi_line_image_with_fill, reports_attributes_as_json)
{
    munin::image image(std::vector<terminalpp::string>{"test", "success"}, 'Q');

    nlohmann::json json = image.to_json();
    ASSERT_EQ("image", json["type"]);
    ASSERT_EQ(2, json["content"]["size"]);
    ASSERT_EQ("test", json["content"]["content"][0]);
    ASSERT_EQ("success", json["content"]["content"][1]);
    ASSERT_EQ("test\nsuccess", json["name"]);
    ASSERT_EQ("Q", json["fill"]);
}

TEST(an_image, reports_its_current_bounds_as_json)
{
    munin::image image;
    image.set_position({3, 5});
    image.set_size({7, 2});

    nlohmann::json json = image.to_json();

    ASSERT_EQ(3, json["position"]["x"]);
    ASSERT_EQ(5, json["position"]["y"]);
    ASSERT_EQ(7, json["size"]["width"]);
    ASSERT_EQ(2, json["size"]["height"]);
}

TEST(an_image, reports_its_automation_id_as_json)
{
    munin::image image;
    image.set_id("status_image");

    nlohmann::json json = image.to_json();

    ASSERT_EQ("status_image", json["id"]);
}

TEST(an_image, reports_its_preferred_size_as_json)
{
    munin::image image(std::vector<terminalpp::string>{"ok", "ready"});

    nlohmann::json json = image.to_json();

    ASSERT_EQ(5, json["preferred_size"]["width"]);
    ASSERT_EQ(2, json["preferred_size"]["height"]);
}

TEST(an_image, reports_its_default_focus_and_cursor_state_as_json)
{
    munin::image image;

    nlohmann::json json = image.to_json();

    ASSERT_EQ(false, json["has_focus"]);
    ASSERT_EQ(false, json["cursor_state"]);
    ASSERT_EQ(0, json["cursor_position"]["x"]);
    ASSERT_EQ(0, json["cursor_position"]["y"]);
}

TEST(an_image, reports_when_it_has_focus_as_json)
{
    munin::image image;
    image.set_can_receive_focus(true);
    image.set_focus();

    nlohmann::json json = image.to_json();

    ASSERT_EQ(true, json["has_focus"]);
}

TEST(an_image, reports_changed_fill_as_json)
{
    munin::image image;
    image.set_fill('Z');

    nlohmann::json json = image.to_json();

    ASSERT_EQ("Z", json["fill"]);
}

TEST(an_image, reports_changed_single_line_content_as_json)
{
    munin::image image;
    image.set_content("Ready");

    nlohmann::json json = image.to_json();

    ASSERT_EQ(1, json["content"]["size"]);
    ASSERT_EQ("Ready", json["content"]["content"][0]);
    ASSERT_EQ("Ready", json["name"]);
}

TEST(an_image, reports_changed_multi_line_content_as_json)
{
    munin::image image("Ready");
    image.set_content(std::vector<terminalpp::string>{"First", "Second"});

    nlohmann::json json = image.to_json();

    ASSERT_EQ(2, json["content"]["size"]);
    ASSERT_EQ("First", json["content"]["content"][0]);
    ASSERT_EQ("Second", json["content"]["content"][1]);
    ASSERT_EQ("First\nSecond", json["name"]);
}

TEST(an_image, reports_default_content_after_content_is_cleared_as_json)
{
    munin::image image("Ready");
    image.set_content();

    nlohmann::json json = image.to_json();

    ASSERT_EQ(0, json["content"]["size"]);
    ASSERT_EQ(json.end(), json.find("name"));
}
