#include "container_test.hpp"
#include <tuple>

using testing::Return;
using testing::TestWithParam;
using testing::ValuesIn;

TEST_F(a_container_with_one_component, has_no_cursor)
{
    EXPECT_CALL(*component, do_has_focus())
        .WillRepeatedly(Return(false));

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

TEST_P(containers_with_cursors, returns_the_state_and_position_of_the_focussed_component_relative_to_the_component_position)
{
    auto const &param = GetParam();
    auto const &cursor_state       = std::get<0>(param);
    auto const &cursor_position    = std::get<1>(param);
    auto const &component_position = std::get<2>(param);
    auto const &expected_result    = std::get<3>(param);

    EXPECT_CALL(*component, do_has_focus())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component, do_get_cursor_state())
        .WillRepeatedly(Return(cursor_state));

    EXPECT_CALL(*component, do_get_cursor_position())
        .WillRepeatedly(Return(cursor_position));

    EXPECT_CALL(*component, do_get_position())
        .WillRepeatedly(Return(component_position));

    ASSERT_EQ(cursor_state, container.get_cursor_state());
    ASSERT_EQ(expected_result, container.get_cursor_position());
}

INSTANTIATE_TEST_CASE_P(
    test,
    containers_with_cursors,
    ValuesIn(std::vector<cursor_state_test_data>{
        cursor_state_test_data {
            false,
            { 0, 0 },
            { 0, 0 },
            { 0, 0 }
        },
        cursor_state_test_data {
            true,
            { 0, 0 },
            { 0, 0 },
            { 0, 0 }
        },
        cursor_state_test_data {
            false,
            { 10, 10 },
            { 0, 0 },
            { 10, 10 }
        },
        cursor_state_test_data {
            true,
            { 10, 10 },
            { 0, 0 },
            { 10, 10 }
        },
        cursor_state_test_data {
            true,
            { 10, 10 },
            { 5, 5 },
            { 15, 15 }
        }
    }));

TEST_F(a_container_with_two_components_where_the_last_has_focus, skips_the_first_when_looking_for_cursor_state_and_position)
{
    EXPECT_CALL(*component0, do_has_focus())
        .WillRepeatedly(Return(false));

    EXPECT_CALL(*component1, do_has_focus())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component1, do_get_cursor_state())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component1, do_get_cursor_position())
        .WillRepeatedly(Return(terminalpp::point(3, 4)));

    EXPECT_CALL(*component1, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(7, 8)));

    ASSERT_TRUE(container.get_cursor_state());
    ASSERT_EQ(terminalpp::point(10, 12), container.get_cursor_position());
}

TEST_F(a_container_with_one_component, does_nothing_when_setting_cursor_position)
{
    container.set_cursor_position({0, 0});
}

TEST_F(a_container_with_one_component_that_has_focus, sets_the_cursor_position_of_the_focussed_subcomponent_when_setting_cursor_position)
{
    static terminalpp::point const cursor_position = {5, 5};

    EXPECT_CALL(*component, do_has_focus())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(0, 0)));

    EXPECT_CALL(*component, do_set_cursor_position(cursor_position));

    container.set_cursor_position(cursor_position);
}

TEST_F(a_container_with_one_component_that_has_focus, sets_the_cursor_position_of_the_focussed_subcomponent_relative_to_the_component_position)
{
    static terminalpp::point const component_position = {3, 4};
    static terminalpp::point const cursor_position    = {5, 5};
    static terminalpp::point const expected_position  = {2, 1};

    EXPECT_CALL(*component, do_has_focus())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component, do_get_position())
        .WillRepeatedly(Return(component_position));

    EXPECT_CALL(*component, do_set_cursor_position(expected_position));

    container.set_cursor_position(cursor_position);
}

TEST_F(a_container_with_two_components_where_the_last_has_focus, skips_the_first_component_when_setting_cursor_position)
{
    static terminalpp::point const component_position = {3, 4};
    static terminalpp::point const cursor_position    = {5, 5};
    static terminalpp::point const expected_position  = {2, 1};

    EXPECT_CALL(*component0, do_has_focus())
        .WillRepeatedly(Return(false));

    EXPECT_CALL(*component1, do_has_focus())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*component1, do_get_position())
        .WillRepeatedly(Return(component_position));

    EXPECT_CALL(*component1, do_set_cursor_position(expected_position));

    container.set_cursor_position(cursor_position);
}
