#include <gtest/gtest.h>
#include <munin/edit.hpp>

using namespace terminalpp::literals;  // NOLINT

class an_edit_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return edit_->to_json();
    }

    std::shared_ptr<munin::edit> edit_ = munin::make_edit();
};

TEST_F(an_edit_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("edit", json.at("type"));
}

TEST_F(an_edit_json_snapshot, reports_its_automation_id)
{
    edit_->set_id("name_edit");

    auto const json = current_json();

    ASSERT_EQ("name_edit", json.at("id"));
}

TEST_F(an_edit_json_snapshot, reports_its_current_bounds)
{
    edit_->set_position({3, 5});
    edit_->set_size({7, 2});

    auto const json = current_json();

    ASSERT_EQ(3, json.at("position").at("x"));
    ASSERT_EQ(5, json.at("position").at("y"));
    ASSERT_EQ(7, json.at("size").at("width"));
    ASSERT_EQ(2, json.at("size").at("height"));
}

TEST_F(an_edit_json_snapshot, reports_its_preferred_size)
{
    auto const json = current_json();

    ASSERT_EQ(1, json.at("preferred_size").at("width"));
    ASSERT_EQ(1, json.at("preferred_size").at("height"));
}

TEST_F(an_edit_json_snapshot, reports_when_it_does_not_have_focus)
{
    auto const json = current_json();

    ASSERT_EQ(false, json.at("has_focus"));
}

TEST_F(an_edit_json_snapshot, reports_when_it_has_focus)
{
    edit_->set_focus();

    auto const json = current_json();

    ASSERT_EQ(true, json.at("has_focus"));
}

TEST_F(an_edit_json_snapshot, reports_empty_text)
{
    auto const json = current_json();

    ASSERT_EQ("", json.at("text"));
}

TEST_F(an_edit_json_snapshot, reports_inserted_text)
{
    edit_->insert_text("ok");

    auto const json = current_json();

    ASSERT_EQ("ok", json.at("text"));
}

TEST_F(an_edit_json_snapshot, reports_set_text)
{
    edit_->set_text("set"_ts);

    auto const json = current_json();

    ASSERT_EQ("set", json.at("text"));
}

TEST_F(an_edit_json_snapshot, reports_caret_position)
{
    edit_->insert_text("ok");

    auto const json = current_json();

    ASSERT_EQ(2, json.at("caret_position"));
}

TEST_F(an_edit_json_snapshot, reports_cursor_after_inserted_text)
{
    edit_->set_size({4, 1});
    edit_->insert_text("ok");

    auto const json = current_json();

    ASSERT_EQ(true, json.at("cursor_state"));
    ASSERT_EQ(2, json.at("cursor_position").at("x"));
    ASSERT_EQ(0, json.at("cursor_position").at("y"));
}
