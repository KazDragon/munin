#include <munin/null_layout.hpp>
#include <gtest/gtest.h>

TEST(a_null_layout, reports_attributes_as_json)
{
  munin::null_layout nl;
  munin::layout &lyt = nl;

  nlohmann::json json = lyt.to_json();
  ASSERT_EQ("null_layout", json["type"]);
}
