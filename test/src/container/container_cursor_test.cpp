#include "container_test.hpp"
#include <tuple>

using testing::InSequence;
using testing::Return;
using testing::ValuesIn;

TEST_F(a_container_with_one_component, has_no_cursor)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  ASSERT_FALSE(container.get_cursor_state());
  ASSERT_EQ(terminalpp::point(0, 0), container.get_cursor_position());
}

using cursor_state_test_data = std::tuple<
    bool,               // Cursor State
    terminalpp::point,  // Cursor Position
    terminalpp::point,  // Component Position
    terminalpp::point   // Expected Result
    >;

using containers_with_cursors =
    containers_with_a_focussed_component<cursor_state_test_data>;

TEST_P(
    containers_with_cursors,
    returns_the_state_and_position_of_the_focussed_component_relative_to_the_component_position)
{
  auto const &param = GetParam();
  auto const &cursor_state = std::get<0>(param);
  auto const &cursor_position = std::get<1>(param);
  auto const &component_position = std::get<2>(param);
  auto const &expected_result = std::get<3>(param);

  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component, do_get_cursor_state())
      .WillRepeatedly(Return(cursor_state));

  EXPECT_CALL(*component, do_get_cursor_position())
      .WillRepeatedly(Return(cursor_position));

  EXPECT_CALL(*component, do_get_position())
      .WillRepeatedly(Return(component_position));

  ASSERT_EQ(cursor_state, container.get_cursor_state());
  ASSERT_EQ(expected_result, container.get_cursor_position());
}

INSTANTIATE_TEST_SUITE_P(
    test,
    containers_with_cursors,
    ValuesIn(std::vector<cursor_state_test_data>{
        cursor_state_test_data{false, {0, 0}, {0, 0}, {0, 0}},
        cursor_state_test_data{true, {0, 0}, {0, 0}, {0, 0}},
        cursor_state_test_data{false, {10, 10}, {0, 0}, {10, 10}},
        cursor_state_test_data{true, {10, 10}, {0, 0}, {10, 10}},
        cursor_state_test_data{true, {10, 10}, {5, 5}, {15, 15}}}));

TEST_F(
    a_container_with_two_components_where_the_last_has_focus,
    skips_the_first_when_looking_for_cursor_state_and_position)
{
  EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));

  EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component1, do_get_cursor_state()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component1, do_get_cursor_position())
      .WillRepeatedly(Return(terminalpp::point(3, 4)));

  EXPECT_CALL(*component1, do_get_position())
      .WillRepeatedly(Return(terminalpp::point(7, 8)));

  ASSERT_TRUE(container.get_cursor_state());
  ASSERT_EQ(terminalpp::point(10, 12), container.get_cursor_position());
}

TEST_F(
    a_container_with_one_component, does_nothing_when_setting_cursor_position)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  container.set_cursor_position({0, 0});
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    sets_the_cursor_position_of_the_focussed_subcomponent_when_setting_cursor_position)
{
  static terminalpp::point const cursor_position = {5, 5};

  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component, do_get_position())
      .WillRepeatedly(Return(terminalpp::point(0, 0)));

  EXPECT_CALL(*component, do_set_cursor_position(cursor_position));

  container.set_cursor_position(cursor_position);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    sets_the_cursor_position_of_the_focussed_subcomponent_relative_to_the_component_position)
{
  static terminalpp::point const component_position = {3, 4};
  static terminalpp::point const cursor_position = {5, 5};
  static terminalpp::point const expected_position = {2, 1};

  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component, do_get_position())
      .WillRepeatedly(Return(component_position));

  EXPECT_CALL(*component, do_set_cursor_position(expected_position));

  container.set_cursor_position(cursor_position);
}

TEST_F(
    a_container_with_two_components_where_the_last_has_focus,
    skips_the_first_component_when_setting_cursor_position)
{
  static terminalpp::point const component_position = {3, 4};
  static terminalpp::point const cursor_position = {5, 5};
  static terminalpp::point const expected_position = {2, 1};

  EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));

  EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component1, do_get_position())
      .WillRepeatedly(Return(component_position));

  EXPECT_CALL(*component1, do_set_cursor_position(expected_position));

  container.set_cursor_position(cursor_position);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_position_change_event_when_cursor_position_of_subcomponent_changes)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  component->on_cursor_position_changed();
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    emits_cursor_position_change_event_when_cursor_position_of_subcomponent_changes)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  component->on_cursor_position_changed();
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    does_not_emit_cursor_position_change_when_unfocused_component_changes_cursor_position)
{
  EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(false));

  component1->on_cursor_position_changed();
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_state_changed_event_when_cursor_state_of_subcomponent_changes)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  component->on_cursor_state_changed();
  ASSERT_EQ(0, cursor_state_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    emits_cursor_state_changed_event_when_subcomponent_cursor_state_changes)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  component->on_cursor_state_changed();
  ASSERT_EQ(1, cursor_state_changed_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    does_not_emit_cursor_state_changed_event_when_unfocused_component_changes_cursor_state)
{
  EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(true));

  EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(false));

  component1->on_cursor_state_changed();
  ASSERT_EQ(0, cursor_state_changed_count);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_events_when_subcomponent_refuses_focus)
{
  EXPECT_CALL(*component, do_set_focus());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  container.set_focus();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component,
    emits_cursor_events_when_subcomponent_accepts_focus)
{
  EXPECT_CALL(*component, do_set_focus());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  container.set_focus();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_events_when_subcomponent_refuses_next_focus)
{
  EXPECT_CALL(*component, do_focus_next());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  container.focus_next();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component,
    emits_cursor_events_when_subcomponent_accepts_next_focus)
{
  EXPECT_CALL(*component, do_focus_next());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  container.focus_next();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    does_not_emit_cursor_events_when_subcomponent_does_not_yield_next_focus)
{
  EXPECT_CALL(*component, do_focus_next());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  container.focus_next();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    emits_cursor_events_when_subcomponent_yields_next_focus)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  {
    InSequence s1;
    EXPECT_CALL(*component, do_focus_next());
    EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));
  }

  container.focus_next();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    emits_cursor_events_when_next_focus_moves_from_one_component_to_the_next)
{
  {
    InSequence s1;
    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));

    EXPECT_CALL(*component1, do_focus_next());
    EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(true));
  }

  container.focus_next();

  ASSERT_EQ(1, cursor_position_changed_count);
  ASSERT_EQ(1, cursor_state_changed_count);
}

//

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_events_when_subcomponent_refuses_previous_focus)
{
  EXPECT_CALL(*component, do_focus_previous());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  container.focus_previous();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component,
    emits_cursor_events_when_subcomponent_accepts_previous_focus)
{
  EXPECT_CALL(*component, do_focus_previous());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  container.focus_previous();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    does_not_emit_cursor_events_when_subcomponent_does_not_yield_previous_focus)
{
  EXPECT_CALL(*component, do_focus_previous());
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  container.focus_previous();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    emits_cursor_events_when_subcomponent_yields_previous_focus)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  {
    InSequence s1;
    EXPECT_CALL(*component, do_focus_previous());
    EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));
  }

  container.focus_previous();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}

TEST_F(
    a_container_with_two_components_where_the_last_has_focus,
    emits_cursor_events_when_next_focus_moves_from_one_component_to_the_previous)
{
  {
    InSequence s1;
    EXPECT_CALL(*component1, do_focus_previous());
    EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(false));

    EXPECT_CALL(*component0, do_focus_previous());
    EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(true));
  }

  container.focus_previous();

  ASSERT_EQ(1, cursor_position_changed_count);
  ASSERT_EQ(1, cursor_state_changed_count);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_cursor_events_when_losing_focus)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  container.lose_focus();

  ASSERT_EQ(0, cursor_state_changed_count);
  ASSERT_EQ(0, cursor_position_changed_count);
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    emits_cursor_events_when_losing_focus)
{
  {
    InSequence s1;
    EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));
    EXPECT_CALL(*component, do_lose_focus()).Times(1);
  }

  container.lose_focus();

  ASSERT_EQ(1, cursor_state_changed_count);
  ASSERT_EQ(1, cursor_position_changed_count);
}
