#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

TEST(setting_the_size_of_a_basic_component_to_a_new_value, sets_and_announces_that_value)
{
    fake_basic_component component;
    int size_changed_called = 0;

    component.on_size_changed.connect(
        [&]()
        {
            ++size_changed_called;
        });
        
    terminalpp::extent expected_size = {10, 15};
    
    component.set_size(expected_size);
    
    ASSERT_EQ(1, size_changed_called);
    ASSERT_EQ(expected_size, component.get_size());
}