#include "container_test.hpp"

using testing::_;
using testing::Invoke;
using testing::Return;

TEST_F(a_container_with_one_component, does_not_forward_events_to_the_subcomponent)
{
    // Because the component does not have focus, it does not make sense for
    // the component to receive the events.
    container.event('X');
}

TEST_F(a_container_with_one_component_that_has_focus, forwards_events_to_the_subcomponent)
{
    EXPECT_CALL(*component, do_has_focus())
        .WillOnce(Return(true));

    EXPECT_CALL(*component, do_event(_))
        .WillOnce(Invoke([](boost::any event)
        {
            char *p = boost::any_cast<char>(&event);
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
        .WillOnce(Invoke([](boost::any event)
        {
            char *p = boost::any_cast<char>(&event);
            ASSERT_NE(nullptr, p);
            ASSERT_EQ('X', *p);
        }));
    container.event('X');
}
