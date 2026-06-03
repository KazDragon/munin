#include <gtest/gtest.h>
#include <munin/list.hpp>

using namespace terminalpp::literals;  // NOLINT

class a_list_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return list_.to_json();
    }

    munin::list list_;
};

TEST_F(a_list_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("list", json.at("type"));
}

TEST_F(a_list_json_snapshot, reports_empty_items)
{
    auto const json = current_json();

    ASSERT_EQ(0, json.at("items").at("size"));
}

TEST_F(a_list_json_snapshot, reports_item_count)
{
    list_.set_items({"first"_ts});

    auto const json = current_json();

    ASSERT_EQ(1, json.at("items").at("size"));
}

TEST_F(a_list_json_snapshot, reports_item_content)
{
    list_.set_items({"first"_ts});

    auto const json = current_json();

    ASSERT_EQ("first", json.at("items").at("content").at(0));
}

TEST_F(a_list_json_snapshot, reports_the_second_item_content)
{
    list_.set_items({"first"_ts, "second"_ts});

    auto const json = current_json();

    ASSERT_EQ("second", json.at("items").at("content").at(1));
}

TEST_F(a_list_json_snapshot, reports_no_selected_item)
{
    auto const json = current_json();

    ASSERT_TRUE(json.at("selected_item_index").is_null());
}

TEST_F(a_list_json_snapshot, reports_the_selected_item_index)
{
    list_.set_items({"first"_ts, "second"_ts});
    list_.select_item(1);

    auto const json = current_json();

    ASSERT_EQ(1, json.at("selected_item_index"));
}
