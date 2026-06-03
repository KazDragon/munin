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

TEST_F(a_text_area_json_snapshot, reports_inserted_text)
{
    text_area_.insert_text("ok"_ts);

    auto const json = current_json();

    ASSERT_EQ("ok", json.at("text"));
}
