#include "viewport_test.hpp"
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/canvas.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Return;
using testing::ValuesIn;
using testing::_;

namespace {

using viewport_cursor_test_data = std::tuple<
    terminalpp::point,     // origin tracked cursor position
    terminalpp::point,     // expected viewport origin cursor position
    terminalpp::point,     // destination tracked cursor position
    terminalpp::point,     // expected destination viewport cursor position
    terminalpp::rectangle  // expected tracked draw area
>;

class viewport_cursor_tracking_test
  : public a_viewport_with_mock_tracked_component,
    public testing::TestWithParam<viewport_cursor_test_data>
{
protected:
    viewport_cursor_tracking_test()
    {
        static auto constexpr tracked_component_preferred_size = terminalpp::extent{6, 6};
        static auto constexpr viewport_size = terminalpp::extent{3, 3};

        ON_CALL(*tracked_component_, do_get_preferred_size())
            .WillByDefault(Return(tracked_component_preferred_size));
        tracked_component_->on_preferred_size_changed();

        viewport_->set_size(viewport_size);
    }
};

}

TEST_P(viewport_cursor_tracking_test, viewports_track_cursor_movement)
{
    auto const &param = GetParam();
    auto const &origin_tracked_cursor_position = std::get<0>(param);
    auto const &expected_viewport_cursor_position = std::get<1>(param);
    auto const &destination_tracked_cursor_position = std::get<2>(param);
    auto const &expected_destination_viewport_cursor_position = std::get<3>(param);
    auto const &expected_tracked_draw_area = std::get<4>(param);

    tracked_component_->set_cursor_position(origin_tracked_cursor_position);
    ASSERT_EQ(expected_viewport_cursor_position, viewport_->get_cursor_position());

    tracked_component_->set_cursor_position(destination_tracked_cursor_position);
    ASSERT_EQ(expected_destination_viewport_cursor_position, viewport_->get_cursor_position());

    EXPECT_CALL(*tracked_component_, do_draw(_, expected_tracked_draw_area));

    terminalpp::canvas cvs{{5, 5}};
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    viewport_->draw(surface, {{}, viewport_->get_size()});
};


INSTANTIATE_TEST_CASE_P(
    viewport_cursor_tracking,
    viewport_cursor_tracking_test,
    ValuesIn({
        // Base case: cursor is unmoved
        viewport_cursor_test_data{
            {0, 0}, {0, 0}, {0, 0}, {0, 0}, {{0, 0}, {3, 3}}
        },
    })
);
