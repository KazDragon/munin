#include <munin/image.hpp>
#include <munin/framed_component.hpp>
#include <munin/solid_frame.hpp>
#include <gtest/gtest.h>

TEST(a_framed_component, reports_attributes_as_json)
{
    auto framed_component = munin::make_framed_component(
        munin::make_solid_frame(),
        munin::make_image("image"));
        
    auto json = framed_component->to_json();
    ASSERT_EQ("framed_component", json["type"]);
    ASSERT_EQ("solid_frame",      json["frame"]["type"]);
    ASSERT_EQ("image",            json["component"]["type"]);
    ASSERT_EQ("framed_layout",    json["layout"]["type"]);
}
