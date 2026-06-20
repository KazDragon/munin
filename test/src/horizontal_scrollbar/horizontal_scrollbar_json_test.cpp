#include <gtest/gtest.h>
#include <munin/horizontal_scrollbar.hpp>

class a_horizontal_scrollbar_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return scrollbar_->to_json();
    }

    std::shared_ptr<munin::horizontal_scrollbar> scrollbar_{
        munin::make_horizontal_scrollbar()};
};

TEST_F(a_horizontal_scrollbar_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("horizontal_scrollbar", json.at("type"));
}

TEST_F(a_horizontal_scrollbar_json_snapshot, reports_its_automation_id)
{
    scrollbar_->set_id("horizontal_scrollbar");

    auto const json = current_json();

    ASSERT_EQ("horizontal_scrollbar", json.at("id"));
}
