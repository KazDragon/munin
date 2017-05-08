#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

class an_enabled_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        assert(component_.is_enabled());
    }
    
    fake_basic_component component_;
};

TEST_F(an_enabled_basic_component, remains_enabled_when_enabled)
{
    component_.enable();
    
    ASSERT_TRUE(component_.is_enabled());
}

TEST_F(an_enabled_basic_component, can_be_disabled)
{
    component_.disable();
    
    ASSERT_FALSE(component_.is_enabled());
}