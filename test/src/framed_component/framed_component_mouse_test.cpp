#include "mock/component.hpp"
#include "mock/frame.hpp"

#include <gtest/gtest.h>
#include <munin/framed_component.hpp>
#include <terminalpp/mouse.hpp>

using testing::_;
using testing::Return;
using testing::ReturnPointee;
using testing::SaveArg;
using testing::ValuesIn;

using framed_component_mouse_data = std::tuple<
    terminalpp::extent,  // size of framed component
    terminalpp::point,   // click position
    terminalpp::point    // expected click position on inner component
    >;

class framed_components
  : public testing::TestWithParam<framed_component_mouse_data>
{
protected:
    framed_components()
    {
        // Fake out the position and size functions for the frame and inner
        // components.
        ON_CALL(*mock_frame_, do_set_position(_))
            .WillByDefault(SaveArg<0>(&mock_frame_position_));
        ON_CALL(*mock_frame_, do_get_position())
            .WillByDefault(ReturnPointee(&mock_frame_position_));

        ON_CALL(*mock_frame_, do_set_size(_))
            .WillByDefault(SaveArg<0>(&mock_frame_size_));
        ON_CALL(*mock_frame_, do_get_size())
            .WillByDefault(ReturnPointee(&mock_frame_size_));

        ON_CALL(*mock_inner_, do_set_position(_))
            .WillByDefault(SaveArg<0>(&mock_inner_position_));
        ON_CALL(*mock_inner_, do_get_position())
            .WillByDefault(ReturnPointee(&mock_inner_position_));

        ON_CALL(*mock_inner_, do_set_size(_))
            .WillByDefault(SaveArg<0>(&mock_inner_size_));
        ON_CALL(*mock_inner_, do_get_size())
            .WillByDefault(ReturnPointee(&mock_inner_size_));
    }

    std::shared_ptr<mock_frame> mock_frame_{make_mock_frame()};
    std::shared_ptr<mock_component> mock_inner_{make_mock_component()};
    std::shared_ptr<munin::framed_component> framed_component_{
        make_framed_component(mock_frame_, mock_inner_)};

private:
    terminalpp::point mock_frame_position_;
    terminalpp::extent mock_frame_size_;

    terminalpp::point mock_inner_position_;
    terminalpp::extent mock_inner_size_;
};

TEST_P(framed_components, forward_mouse_clicks_to_the_inner_component)
{
    auto const &params = GetParam();
    auto const &component_size = std::get<0>(params);
    auto const &initial_click = std::get<1>(params);
    auto const &expected_click = std::get<2>(params);

    ON_CALL(*mock_frame_, north_border_height())
        .WillByDefault(Return(terminalpp::coordinate_type{1}));
    ON_CALL(*mock_frame_, south_border_height())
        .WillByDefault(Return(terminalpp::coordinate_type{1}));
    ON_CALL(*mock_frame_, west_border_width())
        .WillByDefault(Return(terminalpp::coordinate_type{1}));
    ON_CALL(*mock_frame_, east_border_width())
        .WillByDefault(Return(terminalpp::coordinate_type{1}));

    framed_component_->set_size(component_size);

    auto received_mouse_event = terminalpp::mouse::event{};
    EXPECT_CALL(*mock_inner_, do_event(_))
        .WillOnce([&received_mouse_event](auto ev) {
            auto *mouse_event = boost::any_cast<terminalpp::mouse::event>(&ev);

            if (mouse_event)
            {
                received_mouse_event = *mouse_event;
            }
        });

    auto const sent_mouse_report = terminalpp::mouse::event{
        terminalpp::mouse::event_type::left_button_down, initial_click};

    framed_component_->event(sent_mouse_report);

    auto const expected_mouse_report = terminalpp::mouse::event{
        terminalpp::mouse::event_type::left_button_down, expected_click};

    ASSERT_EQ(expected_mouse_report, received_mouse_event);
}

INSTANTIATE_TEST_SUITE_P(
    framed_component_mouse_clicks,
    framed_components,
    ValuesIn({// General test case - click on the inner component forwards to
              // the inner component's co-ordinate space.
              framed_component_mouse_data({4, 4}, {2, 2}, {1, 1}),

              // A click occurs on the northern border, and is forwarded on to
              // the northernmost part of the inner component.
              framed_component_mouse_data({4, 4}, {2, 0}, {1, 0}),

              // A click occurs on the southern border, and is forwarded on to
              // the southernmost part of the inner component.
              framed_component_mouse_data({4, 4}, {2, 3}, {1, 1}),

              // A click occurs on the western border, and is forwarded on to
              // the westernmost part of the inner component.
              framed_component_mouse_data({4, 4}, {0, 2}, {0, 1}),

              // A click occurs on the eastern border, and is forwarded on to
              // the easternmost part of the inner component.
              framed_component_mouse_data({4, 4}, {3, 2}, {1, 1}),

              // Degenerate case: somehow clicking on a component with no size
              // will forward onto the origin.
              framed_component_mouse_data({0, 0}, {0, 0}, {0, 0})}));
