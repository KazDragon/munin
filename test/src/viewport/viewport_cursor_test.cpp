#include "viewport_test.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/canvas.hpp>

using testing::_;
using testing::ValuesIn;

namespace {

using viewport_cursor_test_data = std::tuple<
    terminalpp::point,     // origin tracked cursor position
    terminalpp::point,     // expected viewport origin cursor position
    terminalpp::point,     // destination tracked cursor position
    terminalpp::point,     // expected destination viewport cursor position
    terminalpp::rectangle  // expected tracked draw area
    >;

using viewport_cursor_tracking_test =
    viewport_mock_test_with_data<viewport_cursor_test_data>;

}  // namespace

TEST_P(viewport_cursor_tracking_test, viewports_track_cursor_movement)
{
    auto const &param = GetParam();
    auto const &origin_tracked_cursor_position = std::get<0>(param);
    auto const &expected_viewport_cursor_position = std::get<1>(param);
    auto const &destination_tracked_cursor_position = std::get<2>(param);
    auto const &expected_destination_viewport_cursor_position =
        std::get<3>(param);
    auto const &expected_tracked_draw_area = std::get<4>(param);

    // Set the origin cursor position.
    tracked_component_->set_cursor_position(origin_tracked_cursor_position);
    ASSERT_EQ(
        expected_viewport_cursor_position, viewport_->get_cursor_position());

    // Some (and only some) cursor movements require the real cursor position to
    // change.  Record whether it does, and assert whether it should.
    bool cursor_position_changed = false;
    viewport_->on_cursor_position_changed.connect(
        [&cursor_position_changed] { cursor_position_changed = true; });

    tracked_component_->set_cursor_position(
        destination_tracked_cursor_position);
    ASSERT_EQ(
        expected_destination_viewport_cursor_position,
        viewport_->get_cursor_position());

    auto const cursor_position_expected_to_change =
        expected_viewport_cursor_position
        != expected_destination_viewport_cursor_position;
    ASSERT_EQ(cursor_position_expected_to_change, cursor_position_changed);

    // Draw the viewport and check that the correct part of the tracked area is
    // drawn.
    EXPECT_CALL(*tracked_component_, do_draw(_, expected_tracked_draw_area));

    terminalpp::canvas cvs{
        {5, 5}
    };
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    viewport_->draw(surface, {{}, viewport_->get_size()});
};

INSTANTIATE_TEST_SUITE_P(
    viewport_cursor_tracking,
    viewport_cursor_tracking_test,
    ValuesIn({
  // Base case: cursor is unmoved
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {{0, 0}, {3, 3}}},

 // Cursor is moved within visual zone
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {1, 1}, {1, 1}, {{0, 0}, {3, 3}}},

 // Cursor is moved to the east of the visual zone
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {3, 0}, {2, 0}, {{1, 0}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {4, 0}, {2, 0}, {{2, 0}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {5, 0}, {2, 0}, {{3, 0}, {3, 3}}},

 // Cursor is moved to the south of the visual zone
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {0, 3}, {0, 2}, {{0, 1}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {0, 4}, {0, 2}, {{0, 2}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {0, 5}, {0, 2}, {{0, 3}, {3, 3}}},

 // Cursor is moved south-east of the visual zone.
        viewport_cursor_test_data{
                                  {0, 0}, {0, 0}, {3, 3}, {2, 2}, {{1, 1}, {3, 3}}},

 // Cursor is moved inside the visual zone after the
  // visual zone is moved.
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {4, 1}, {1, 1}, {{3, 0}, {3, 3}}},
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {3, 2}, {0, 2}, {{3, 0}, {3, 3}}},

 // Cursor is moved to the west of the visual zone.
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {2, 0}, {0, 0}, {{2, 0}, {3, 3}}},
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {1, 0}, {0, 0}, {{1, 0}, {3, 3}}},
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {0, 0}, {0, 0}, {{0, 0}, {3, 3}}},

 // Cursor is moved south-west of the visual zone.
        viewport_cursor_test_data{
                                  {5, 0}, {2, 0}, {2, 3}, {0, 2}, {{2, 1}, {3, 3}}},

 // Cursor is moved to the north of the visual zone.
        viewport_cursor_test_data{
                                  {0, 5}, {0, 2}, {0, 2}, {0, 0}, {{0, 2}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 5}, {0, 2}, {0, 1}, {0, 0}, {{0, 1}, {3, 3}}},
        viewport_cursor_test_data{
                                  {0, 5}, {0, 2}, {0, 0}, {0, 0}, {{0, 0}, {3, 3}}},

 // Cursor is moved north-west of the visual zone.
        viewport_cursor_test_data{
                                  {5, 5}, {2, 2}, {1, 1}, {0, 0}, {{1, 1}, {3, 3}}},

 // Cursor is moved north-east of the visual zone.
        viewport_cursor_test_data{
                                  {0, 5}, {0, 2}, {5, 0}, {2, 0}, {{3, 0}, {3, 3}}},
}));
