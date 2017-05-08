#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

class a_disabled_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        component_.disable();
        assert(!component_.is_enabled());
    }
    
    fake_basic_component component_;
};

TEST_F(a_disabled_basic_component, can_be_enabled)
{
    component_.enable();
    
    ASSERT_TRUE(component_.is_enabled());
}

TEST_F(a_disabled_basic_component, remains_disabled_when_disabled)
{
    component_.disable();
    
    ASSERT_FALSE(component_.is_enabled());
}