#include <munin/brush.hpp>
#include <gtest/gtest.h>

TEST(a_brush, can_have_its_preferred_size_set)
{
    int called = 0;
    auto on_preferred_size_changed = [&called]
    {
        ++called;
    };

    munin::brush brush;
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    brush.set_preferred_size({2, 2});

    ASSERT_EQ(terminalpp::extent(2, 2), brush.get_preferred_size());
    ASSERT_EQ(1, called);
}

