#include "titled_frame_test.hpp"
#include <munin/solid_frame.hpp>

TEST_F(a_titled_frame, reports_attributes_as_json)
{
  nlohmann::json json = frame_.to_json();
  ASSERT_EQ("titled_frame", json["type"]);
  ASSERT_EQ("title", json["title"]);
}