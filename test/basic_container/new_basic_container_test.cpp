#include <munin/basic_container.hpp>
#include <gtest/gtest.h>

TEST(a_new_basic_container, has_no_components)
{
    munin::basic_container container;
    ASSERT_EQ(0u, container.get_number_of_components());
}