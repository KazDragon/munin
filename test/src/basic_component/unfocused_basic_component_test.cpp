#include "fake_basic_component.hpp"
#include <munin/basic_component.hpp>
#include <terminalpp/mouse.hpp>
#include <gtest/gtest.h>
#include <cassert>
#include <memory>

namespace {
class an_unfocused_basic_component_that_receives_focus : public testing::Test
{
 protected:
  an_unfocused_basic_component_that_receives_focus()
  {
    assert(!component_.has_focus());

    component_.on_focus_set.connect([this] { ++on_focus_set_count; });

    component_.on_focus_lost.connect([this] { ++on_focus_lost_count; });
  }

  fake_basic_component_that_receives_focus component_;
  int on_focus_set_count = 0;
  int on_focus_lost_count = 0;
};

}  // namespace

TEST_F(
    an_unfocused_basic_component_that_receives_focus,
    gains_focus_when_focus_is_set)
{
  component_.set_focus();

  ASSERT_TRUE(component_.has_focus());
  ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_receives_focus,
    does_nothing_when_losing_focus)
{
  component_.lose_focus();

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_lost_count);
}

TEST_F(
    an_unfocused_basic_component_that_receives_focus,
    gains_focus_when_next_focus_is_set)
{
  component_.focus_next();

  ASSERT_TRUE(component_.has_focus());
  ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_receives_focus,
    gains_focus_when_previous_focus_is_set)
{
  component_.focus_previous();

  ASSERT_TRUE(component_.has_focus());
  ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_receives_focus, sets_focus_on_mouse_down)
{
  terminalpp::mouse::event mouse_event;
  mouse_event.action_ = terminalpp::mouse::event_type::left_button_down;
  mouse_event.position_ = {0, 0};

  component_.event(mouse_event);

  ASSERT_TRUE(component_.has_focus());
  ASSERT_EQ(1, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_receives_focus, does_nothing_on_mouse_up)
{
  terminalpp::mouse::event mouse_event;
  mouse_event.action_ = terminalpp::mouse::event_type::button_up;
  mouse_event.position_ = {0, 0};

  component_.event(mouse_event);

  ASSERT_FALSE(component_.has_focus());
}

namespace {
class an_unfocused_basic_component_that_does_not_receive_focus
  : public testing::Test
{
 protected:
  an_unfocused_basic_component_that_does_not_receive_focus()
  {
    assert(!component_.has_focus());

    component_.on_focus_set.connect([this] { ++on_focus_set_count; });

    component_.on_focus_lost.connect([this] { ++on_focus_lost_count; });
  }

  fake_basic_component_that_does_not_receive_focus component_;
  int on_focus_set_count = 0;
  int on_focus_lost_count = 0;
};

}  // namespace

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_not_gain_focus_when_focus_is_set)
{
  component_.set_focus();

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_nothing_when_losing_focus)
{
  component_.lose_focus();

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_lost_count);
}

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_not_gain_focus_when_next_focus_is_set)
{
  component_.focus_next();

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_not_gain_focus_when_previous_focus_is_set)
{
  component_.focus_previous();

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_not_set_focus_on_mouse_down)
{
  terminalpp::mouse::event mouse_event;
  mouse_event.action_ = terminalpp::mouse::event_type::left_button_down;
  mouse_event.position_ = {0, 0};

  component_.event(mouse_event);

  ASSERT_FALSE(component_.has_focus());
  ASSERT_EQ(0, on_focus_set_count);
}

TEST_F(
    an_unfocused_basic_component_that_does_not_receive_focus,
    does_nothing_on_mouse_up)
{
  terminalpp::mouse::event mouse_event;
  mouse_event.action_ = terminalpp::mouse::event_type::button_up;
  mouse_event.position_ = {0, 0};

  component_.event(mouse_event);

  ASSERT_FALSE(component_.has_focus());
}
