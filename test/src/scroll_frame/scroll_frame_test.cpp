#include <munin/scroll_frame.hpp>
#include <gtest/gtest.h>

TEST(a_scroll_frame, is_a_frame)
{
    std::shared_ptr<munin::scroll_frame> scroll_frame = 
        munin::make_scroll_frame();

    std::shared_ptr<munin::frame> frame = scroll_frame;
}
