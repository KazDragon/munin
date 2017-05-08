#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <gtest/gtest.h>

class a_focused_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        component_.set_focus();
        
        assert(component_.has_focus());

        component_.on_focus_set.connect([this]
        {
            ++on_focus_set_count;
        });
        
        component_.on_focus_lost.connect([this]
        {
            ++on_focus_lost_count;
        });
    }
    
    fake_basic_component component_;
    int on_focus_set_count  = 0;
    int on_focus_lost_count = 0;
};

TEST_F(a_focused_basic_component, does_nothing_when_setting_focus)
{
    component_.set_focus();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(a_focused_basic_component, loses_focus_when_losing_focus)
{
    component_.lose_focus();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(1, on_focus_lost_count);
}

TEST_F(a_focused_basic_component, loses_focus_when_next_focus_is_set)
{
    component_.focus_next();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(1, on_focus_lost_count);
}

TEST_F(a_focused_basic_component, loses_focus_when_previous_focus_is_set)
{
    component_.focus_previous();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(1, on_focus_lost_count);
}
