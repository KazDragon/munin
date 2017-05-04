#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <cassert>

class an_unfocused_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        Test::SetUp();
        
        assert(!component_.has_focus());
    }
    
    fake_basic_component component_;        
};

TEST_F(an_unfocused_basic_component, gains_focus_when_focus_is_set)
{
    int on_focus_set_count = 0;
    component_.on_focus_set.connect([&on_focus_set_count]
        {
            ++on_focus_set_count;
        });
        
    component_.set_focus();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_unfocused_basic_component, does_nothing_when_losing_focus)
{
    int on_focus_lost_count = 0;
    component_.on_focus_lost.connect([&on_focus_lost_count]
        {
            ++on_focus_lost_count;
        });
        
    component_.lose_focus();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_lost_count);
}

TEST_F(an_unfocused_basic_component, gains_focus_when_next_focus_is_set)
{
    int on_focus_set_count = 0;
    component_.on_focus_set.connect([&on_focus_set_count]
        {
            ++on_focus_set_count;
        });
        
    component_.focus_next();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_unfocused_basic_component, gains_focus_when_previous_focus_is_set)
{
    int on_focus_set_count = 0;
    component_.on_focus_set.connect([&on_focus_set_count]
        {
            ++on_focus_set_count;
        });
        
    component_.focus_previous();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}
