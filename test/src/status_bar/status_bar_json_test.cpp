#include "mock/animator.hpp"

#include <gtest/gtest.h>
#include <munin/status_bar.hpp>

using namespace terminalpp::literals;  // NOLINT

class a_status_bar_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return status_bar_->to_json();
    }

    std::shared_ptr<mock_animator> animator_{make_mock_animator()};
    std::shared_ptr<munin::status_bar> status_bar_{
        munin::make_status_bar(*animator_)};
};

TEST_F(a_status_bar_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("status_bar", json.at("type"));
}

TEST_F(a_status_bar_json_snapshot, reports_its_automation_id)
{
    status_bar_->set_id("main_status");

    auto const json = current_json();

    ASSERT_EQ("main_status", json.at("id"));
}

TEST_F(a_status_bar_json_snapshot, reports_its_visible_message_as_name)
{
    status_bar_->set_message("Ready"_ts);

    auto const json = current_json();

    ASSERT_EQ("Ready", json.at("name"));
}
