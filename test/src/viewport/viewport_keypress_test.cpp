#include "viewport_test.hpp"
#include <terminalpp/virtual_key.hpp>
#include <gtest/gtest.h>

using testing::_;
using testing::ReturnPointee;
using testing::SaveArg;

TEST_F(a_viewport, when_receiving_a_pgdn_translates_that_to_a_cursor_set)
{
  viewport_->set_size({3, 3});
  tracked_component_->set_size({9, 9});

  auto const keypress_pgdn = terminalpp::virtual_key{
      terminalpp::vk::pgdn, terminalpp::vk_modifier::none, 1};

  terminalpp::point tracked_cursor_position = terminalpp::point{1, 1};
  ON_CALL(*tracked_component_, do_get_cursor_position())
      .WillByDefault(ReturnPointee(&tracked_cursor_position));
  ON_CALL(*tracked_component_, do_set_cursor_position(_))
      .WillByDefault(SaveArg<0>(&tracked_cursor_position));

  viewport_->event(keypress_pgdn);

  // The pgdn will have been translated to a cursor move down that
  // relates to the height of the viewport.
  auto const expected_cursor_position = terminalpp::point({1, 4});
  ASSERT_EQ(expected_cursor_position, tracked_cursor_position);
}

TEST_F(a_viewport, when_receiving_a_pgup_translates_that_to_a_cursor_set)
{
  viewport_->set_size({3, 3});
  tracked_component_->set_size({9, 9});

  auto const keypress_pgup = terminalpp::virtual_key{
      terminalpp::vk::pgup, terminalpp::vk_modifier::none, 1};

  terminalpp::point tracked_cursor_position = terminalpp::point{4, 4};
  ON_CALL(*tracked_component_, do_get_cursor_position())
      .WillByDefault(ReturnPointee(&tracked_cursor_position));
  ON_CALL(*tracked_component_, do_set_cursor_position(_))
      .WillByDefault(SaveArg<0>(&tracked_cursor_position));

  viewport_->event(keypress_pgup);

  // The pgdn will have been translated to a cursor move down that
  // relates to the height of the viewport.
  auto const expected_cursor_position = terminalpp::point({4, 1});
  ASSERT_EQ(expected_cursor_position, tracked_cursor_position);
}
