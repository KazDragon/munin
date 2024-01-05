#include "solid_frame_test.hpp"
#include <munin/solid_frame.hpp>

TEST_F(a_solid_frame, reports_attributes_as_json)
{
  nlohmann::json json = frame_.to_json();
  ASSERT_EQ("solid_frame", json["type"]);
}