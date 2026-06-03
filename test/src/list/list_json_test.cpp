#include <gtest/gtest.h>
#include <munin/list.hpp>

TEST(a_default_list, reports_its_type_as_json)
{
    munin::list list;

    auto const json = list.to_json();

    ASSERT_EQ("list", json.at("type"));
}
