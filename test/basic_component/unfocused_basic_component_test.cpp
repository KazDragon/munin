#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <terminalpp/ansi/mouse.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <cassert>

class an_enabled_unfocused_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        Test::SetUp();
     
        assert(component_.is_enabled());   
        assert(!component_.has_focus());

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

TEST_F(an_enabled_unfocused_basic_component, gains_focus_when_focus_is_set)
{
    component_.set_focus();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_enabled_unfocused_basic_component, does_nothing_when_losing_focus)
{
    component_.lose_focus();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_lost_count);
}

TEST_F(an_enabled_unfocused_basic_component, gains_focus_when_next_focus_is_set)
{
    component_.focus_next();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_enabled_unfocused_basic_component, gains_focus_when_previous_focus_is_set)
{
    component_.focus_previous();
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_enabled_unfocused_basic_component, sets_focus_on_mouse_down)
{
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.x_position_ = 0;
    mouse_event.y_position_ = 0;
    mouse_event.button_ = terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN;
    
    component_.event(mouse_event);
    
    ASSERT_TRUE(component_.has_focus());
    ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(an_enabled_unfocused_basic_component, does_nothing_on_mouse_up)
{
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.x_position_ = 0;
    mouse_event.y_position_ = 0;
    mouse_event.button_ = terminalpp::ansi::mouse::report::BUTTON_UP;
    
    component_.event(mouse_event);
    
    ASSERT_FALSE(component_.has_focus());
}

class a_disabled_unfocused_basic_component : public testing::Test
{
protected :
    void SetUp() override
    {
        Test::SetUp();
     
        component_.disable();
        
        assert(!component_.is_enabled());   
        assert(!component_.has_focus());
 
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

TEST_F(a_disabled_unfocused_basic_component, does_nothing_when_setting_focus)
{
    component_.set_focus();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(a_disabled_unfocused_basic_component, does_nothing_when_losing_focus)
{
    component_.lose_focus();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_lost_count);
}

TEST_F(a_disabled_unfocused_basic_component, does_nothing_when_next_focus_is_set)
{
    component_.focus_next();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(a_disabled_unfocused_basic_component, does_nothing_when_previous_focus_is_set)
{
    component_.focus_previous();
    
    ASSERT_FALSE(component_.has_focus());
    ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(a_disabled_unfocused_basic_component, does_nothing_on_mouse_down)
{
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.x_position_ = 0;
    mouse_event.y_position_ = 0;
    mouse_event.button_ = terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN;
    
    component_.event(mouse_event);
    
    ASSERT_FALSE(component_.has_focus());
}

TEST_F(a_disabled_unfocused_basic_component, does_nothing_on_mouse_up)
{
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.x_position_ = 0;
    mouse_event.y_position_ = 0;
    mouse_event.button_ = terminalpp::ansi::mouse::report::BUTTON_UP;
    
    component_.event(mouse_event);
    
    ASSERT_FALSE(component_.has_focus());
}
