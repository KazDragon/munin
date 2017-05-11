#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

TEST(the_size_of_a_basic_component, can_be_altered)
{
    fake_basic_component component;
    
    terminalpp::extent const expected_size = {10, 15};
    
    component.set_size(expected_size);
    
    ASSERT_EQ(expected_size, component.get_size());
}

TEST(the_position_of_a_basic_component, can_be_altered)
{
    fake_basic_component component;
    
    terminalpp::point const expected_position = {32, 42};
    
    component.set_position(expected_position);
    
    ASSERT_EQ(expected_position, component.get_position());
}