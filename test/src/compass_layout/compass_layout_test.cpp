#include <munin/compass_layout.hpp>
#include <gtest/gtest.h>

TEST(compass_layout_test, reports_attributes_as_json)
{
    munin::compass_layout cl;
    munin::layout &lyt = cl;

    nlohmann::json json = lyt.to_json();

    ASSERT_EQ("compass_layout", json["type"]);
}
