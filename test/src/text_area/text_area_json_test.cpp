#include <gtest/gtest.h>
#include <munin/text_area.hpp>

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
