#pragma once
#include "mock/component.hpp"
#include <munin/container.hpp>
#include <gtest/gtest.h>

class container_test_base : public testing::Test
{
 protected:
  container_test_base()
  {
    container.on_preferred_size_changed.connect(
        [this]() { ++preferred_size_changed_count; });

    container.on_focus_set.connect([this]() { ++focus_set_count; });

    container.on_focus_lost.connect([this]() { ++focus_lost_count; });

    container.on_cursor_position_changed.connect(
        [this]() { ++cursor_position_changed_count; });

    container.on_cursor_state_changed.connect(
        [this]() { ++cursor_state_changed_count; });

    container.on_redraw.connect([this](auto const &) { ++redraw_count; });
  }

  void ResetCounters()
  {
    preferred_size_changed_count = 0;
    focus_set_count = 0;
    focus_lost_count = 0;
    cursor_position_changed_count = 0;
    cursor_state_changed_count = 0;
    redraw_count = 0;
  }

  munin::container container;

  int preferred_size_changed_count = 0;
  int focus_set_count = 0;
  int focus_lost_count = 0;
  int cursor_position_changed_count = 0;
  int cursor_state_changed_count = 0;
  int redraw_count = 0;
};

class a_container : public container_test_base
{
};

class a_new_container : public container_test_base
{
};

class a_container_with_one_component : public a_container
{
 protected:
  a_container_with_one_component()
  {
    container.add_component(component);

    ResetCounters();
  }

  std::shared_ptr<mock_component> component{make_mock_component()};
};

class a_container_with_one_component_that_has_focus
  : public a_container_with_one_component
{
 protected:
  a_container_with_one_component_that_has_focus()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*component, do_set_focus());
      EXPECT_CALL(*component, do_has_focus()).WillOnce(Return(true));
    }

    container.set_focus();

    assert(container.has_focus());
    ResetCounters();
  }
};

class a_container_with_two_components : public a_container
{
 protected:
  a_container_with_two_components()
  {
    container.add_component(component0);
    container.add_component(component1);
  }

  std::shared_ptr<mock_component> component0{make_mock_component()};
  std::shared_ptr<mock_component> component1{make_mock_component()};
};

class a_container_with_two_components_where_the_first_has_focus
  : public a_container_with_two_components
{
 protected:
  a_container_with_two_components_where_the_first_has_focus()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));
      EXPECT_CALL(*component0, do_focus_next());
      EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(true));
    }

    container.focus_next();

    assert(container.has_focus());
    ResetCounters();
  }
};

class a_container_with_two_components_where_the_last_has_focus
  : public a_container_with_two_components
{
 protected:
  a_container_with_two_components_where_the_last_has_focus()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(false));
      EXPECT_CALL(*component1, do_focus_previous());
      EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(true));
    }

    container.focus_previous();

    assert(container.has_focus());
    ResetCounters();
  }
};

class a_container_with_three_components : public a_container_with_two_components
{
 protected:
  a_container_with_three_components()
  {
    container.add_component(component2);
  }

  std::shared_ptr<mock_component> component2{make_mock_component()};
};

class a_container_with_three_components_where_the_first_has_focus
  : public a_container_with_three_components
{
 protected:
  a_container_with_three_components_where_the_first_has_focus()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));
      EXPECT_CALL(*component0, do_focus_next());
      EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(true));
    }

    container.focus_next();

    assert(container.has_focus());
    ResetCounters();
  }
};

class a_container_with_three_components_where_the_last_has_focus
  : public a_container_with_three_components
{
 protected:
  a_container_with_three_components_where_the_last_has_focus()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*component2, do_has_focus()).WillRepeatedly(Return(false));
      EXPECT_CALL(*component2, do_focus_previous());
      EXPECT_CALL(*component2, do_has_focus()).WillRepeatedly(Return(true));
    }

    container.focus_previous();

    assert(container.has_focus());
    ResetCounters();
  }
};

template <class TestData>
class containers_with_a_component : public testing::TestWithParam<TestData>
{
 protected:
  containers_with_a_component()
  {
    container.add_component(component);
  }

  munin::container container;
  std::shared_ptr<mock_component> component{make_mock_component()};
};

template <class TestData>
class containers_with_a_focussed_component
  : public containers_with_a_component<TestData>
{
 protected:
  containers_with_a_focussed_component()
  {
    using testing::InSequence;
    using testing::Return;

    {
      InSequence s1;
      EXPECT_CALL(*this->component, do_set_focus());
      EXPECT_CALL(*this->component, do_has_focus()).WillOnce(Return(true));
    }

    this->container.set_focus();

    assert(this->container.has_focus());
  }
};
