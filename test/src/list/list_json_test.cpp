#include <gtest/gtest.h>
#include <munin/list.hpp>

using namespace terminalpp::literals;  // NOLINT

TEST(a_default_list, reports_its_type_as_json)
{
    munin::list list;

    auto const json = list.to_json();

    ASSERT_EQ("list", json.at("type"));
}

TEST(a_default_list, reports_empty_items_as_json)
{
    munin::list list;

    auto const json = list.to_json();

    ASSERT_EQ(0, json.at("items").at("size"));
}

TEST(a_list_json_with_an_item, reports_its_item_count_as_json)
{
    munin::list list;
    list.set_items({"first"_ts});

    auto const json = list.to_json();

    ASSERT_EQ(1, json.at("items").at("size"));
}

TEST(a_list_json_with_an_item, reports_its_item_content_as_json)
{
    munin::list list;
    list.set_items({"first"_ts});

    auto const json = list.to_json();

    ASSERT_EQ("first", json.at("items").at("content").at(0));
}

TEST(a_list_json_with_two_items, reports_the_second_item_content_as_json)
{
    munin::list list;
    list.set_items({"first"_ts, "second"_ts});

    auto const json = list.to_json();

    ASSERT_EQ("second", json.at("items").at("content").at(1));
}

TEST(a_default_list, reports_no_selected_item_as_json)
{
    munin::list list;

    auto const json = list.to_json();

    ASSERT_TRUE(json.at("selected_item_index").is_null());
}
