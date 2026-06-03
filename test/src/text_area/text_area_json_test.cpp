#include <gtest/gtest.h>
#include <munin/text_area.hpp>

using namespace terminalpp::literals;  // NOLINT

class a_text_area_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return text_area_.to_json();
    }

    munin::text_area text_area_;
};

TEST_F(a_text_area_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("text_area", json.at("type"));
}

TEST_F(a_text_area_json_snapshot, reports_its_automation_id)
{
    text_area_.set_id("message_body");

    auto const json = current_json();

    ASSERT_EQ("message_body", json.at("id"));
}

TEST_F(a_text_area_json_snapshot, reports_its_current_bounds)
{
    text_area_.set_position({3, 5});
    text_area_.set_size({7, 2});

    auto const json = current_json();

    ASSERT_EQ(3, json.at("position").at("x"));
    ASSERT_EQ(5, json.at("position").at("y"));
    ASSERT_EQ(7, json.at("size").at("width"));
    ASSERT_EQ(2, json.at("size").at("height"));
}

TEST_F(a_text_area_json_snapshot, reports_its_preferred_size)
{
    auto const json = current_json();

    ASSERT_EQ(0, json.at("preferred_size").at("width"));
    ASSERT_EQ(1, json.at("preferred_size").at("height"));
}

TEST_F(a_text_area_json_snapshot, reports_when_it_does_not_have_focus)
{
    auto const json = current_json();

    ASSERT_EQ(false, json.at("has_focus"));
}

TEST_F(a_text_area_json_snapshot, reports_when_it_has_focus)
{
    text_area_.set_focus();

    auto const json = current_json();

    ASSERT_EQ(true, json.at("has_focus"));
}

TEST_F(a_text_area_json_snapshot, reports_inserted_text)
{
    text_area_.insert_text("ok"_ts);

    auto const json = current_json();

    ASSERT_EQ("ok", json.at("text"));
}

TEST_F(a_text_area_json_snapshot, reports_caret_position)
{
    text_area_.insert_text("ok"_ts);

    auto const json = current_json();

    ASSERT_EQ(2, json.at("caret_position"));
}

TEST_F(a_text_area_json_snapshot, reports_cursor_after_inserted_text)
{
    text_area_.set_size({4, 1});
    text_area_.insert_text("ok"_ts);

    auto const json = current_json();

    ASSERT_EQ(true, json.at("cursor_state"));
    ASSERT_EQ(2, json.at("cursor_position").at("x"));
    ASSERT_EQ(0, json.at("cursor_position").at("y"));
}
