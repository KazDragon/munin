#include "container_test.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <tuple>
#include <vector>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::TestWithParam;
using testing::ValuesIn;

TEST_F(a_container_with_one_component, does_not_forward_events_to_the_subcomponent)
{
    // Because the component does not have focus, it does not make sense for
    // the component to receive the events.
    EXPECT_CALL(*component, do_has_focus())
        .WillRepeatedly(Return(false));

    container.event('X');
}

TEST_F(a_container_with_one_component_that_has_focus, forwards_events_to_the_subcomponent)
{
    EXPECT_CALL(*component, do_has_focus())
        .WillOnce(Return(true));

    EXPECT_CALL(*component, do_event(_))
        .WillOnce(Invoke([](boost::any const &event)
        {
            char const *p = boost::any_cast<char>(&event);
            ASSERT_NE(nullptr, p);
            ASSERT_EQ('X', *p);
        }));
    container.event('X');
}

TEST_F(a_container_with_two_components_where_the_last_has_focus, skips_the_first_and_forwards_events_to_the_last_subcomponent)
{
    EXPECT_CALL(*component0, do_has_focus())
        .WillOnce(Return(false));

    EXPECT_CALL(*component1, do_has_focus())
        .WillOnce(Return(true));

    EXPECT_CALL(*component1, do_event(_))
        .WillOnce(Invoke([](boost::any const &event)
        {
            char const *p = boost::any_cast<char>(&event);
            ASSERT_NE(nullptr, p);
            ASSERT_EQ('X', *p);
        }));
    container.event('X');
}

TEST_F(a_container_with_one_component, forwards_mouse_events_even_though_the_component_has_no_focus)
{
    static terminalpp::ansi::mouse::report const report {
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
        0,
        0
    };

    EXPECT_CALL(*component, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(0, 0)));

    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component, do_event(_))
        .WillOnce(Invoke([](boost::any const &event)
        {
            auto *p = boost::any_cast<terminalpp::ansi::mouse::report>(&event);
            ASSERT_NE(nullptr, p);
            ASSERT_EQ(report, *p);
        }));

    container.event(report);
}

using mouse_report_test_data = std::tuple<
    terminalpp::point,                  // Position of component
    terminalpp::extent,                 // Size of component
    terminalpp::ansi::mouse::report,    // Report as received by component
    terminalpp::ansi::mouse::report>;   // Expected forwarded report

using containers_forwarding_mouse_events =
    containers_with_a_component<mouse_report_test_data>;

TEST_P(containers_forwarding_mouse_events, translate_coordinates_relative_to_component_position)
{
    auto const &param = GetParam();
    auto const &component_position = std::get<0>(param);
    auto const &component_size     = std::get<1>(param);
    auto const &report             = std::get<2>(param);
    auto const &expected_value     = std::get<3>(param);

    EXPECT_CALL(*component, do_get_position())
        .WillRepeatedly(Return(component_position));

    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(component_size));

    EXPECT_CALL(*component, do_event(_))
        .WillOnce(Invoke([&expected_value](boost::any event)
        {
            auto *report = boost::any_cast<terminalpp::ansi::mouse::report>(&event);
            ASSERT_NE(nullptr, report);
            ASSERT_EQ(expected_value, *report);
        }));
    container.event(report);
}

INSTANTIATE_TEST_CASE_P(
    test,
    containers_forwarding_mouse_events,
    ValuesIn(std::vector<mouse_report_test_data>{
        mouse_report_test_data {
            {0, 0},
            {20, 20},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 15},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 15}
        },
        mouse_report_test_data {
            {10, 0},
            {20, 20},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 15},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 5, 15}
        },
        mouse_report_test_data {
            {0, 10},
            {20, 20},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 15},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 5}
        },
        mouse_report_test_data {
            {10, 10},
            {20, 20},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 15, 15},
            {terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN, 5,  5}
        }
    }));

TEST_F(a_container_with_two_components, skips_components_that_are_not_at_the_mouse_report_location)
{
    static terminalpp::ansi::mouse::report const report = {
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
        20,
        20
    };

    static terminalpp::ansi::mouse::report const expected_value = {
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
        5,
        5
    };

    EXPECT_CALL(*component0, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 0)));

    EXPECT_CALL(*component0, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component1, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(15, 15)));

    EXPECT_CALL(*component1, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component1, do_event(_))
        .WillOnce(Invoke([](boost::any event)
        {
            auto *report = boost::any_cast<terminalpp::ansi::mouse::report>(&event);
            ASSERT_NE(nullptr, report);
            ASSERT_EQ(expected_value, *report);
        }));

    container.event(report);
}
