#include <gtest/gtest.h>
#include <munin/filled_box.hpp>
#include <munin/framed_component.hpp>
#include <munin/solid_frame.hpp>

TEST(a_new_framed_component, is_constructed_from_a_frame_and_a_component)
{
    std::shared_ptr<munin::framed_component> comp = munin::make_framed_component(
        munin::make_solid_frame(),
        munin::make_fill('X')
    );
}
