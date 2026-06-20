#include <gtest/gtest.h>
#include <munin/vertical_scrollbar.hpp>

class a_vertical_scrollbar_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return scrollbar_->to_json();
    }

    std::shared_ptr<munin::vertical_scrollbar> scrollbar_{
        munin::make_vertical_scrollbar()};
};

TEST_F(a_vertical_scrollbar_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("vertical_scrollbar", json.at("type"));
}

TEST_F(a_vertical_scrollbar_json_snapshot, reports_its_automation_id)
{
    scrollbar_->set_id("vertical_scrollbar");

    auto const json = current_json();

    ASSERT_EQ("vertical_scrollbar", json.at("id"));
}
