#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

TEST(a_new_basic_component, has_a_position_at_the_origin)
{
    fake_basic_component comp;
    ASSERT_EQ(terminalpp::point(0, 0), comp.get_position());
}

TEST(a_new_basic_component, has_a_size_of_zero)
{
    fake_basic_component comp;
    ASSERT_EQ(terminalpp::extent(0, 0), comp.get_size());
}

TEST(a_new_basic_component, can_receive_focus)
{
    fake_basic_component comp;
    ASSERT_TRUE(comp.can_focus());
}

TEST(a_new_basic_component, does_not_have_focus)
{
    fake_basic_component comp;
    ASSERT_FALSE(comp.has_focus());
}

TEST(a_new_basic_component, is_enabled)
{
    fake_basic_component comp;
    ASSERT_TRUE(comp.is_enabled());
}

TEST(a_new_basic_component, has_no_cursor)
{
    fake_basic_component comp;
    ASSERT_FALSE(comp.get_cursor_state());
    ASSERT_EQ(terminalpp::point(0, 0), comp.get_cursor_position());
}