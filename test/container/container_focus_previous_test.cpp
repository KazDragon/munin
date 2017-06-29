#include "container_test.hpp"

using testing::InSequence;
using testing::Return;

TEST_F(a_container_with_one_component, calls_focus_previous_on_subcomponent_on_focus_previous)
{
    {
        InSequence s1;
        EXPECT_CALL(*component, do_focus_previous());
        EXPECT_CALL(*component, do_has_focus())
            .WillOnce(Return(true));
    }

    container.focus_previous();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_one_component, that_refuses_focus_refuses_focus_on_focus_previous)
{
    {
        InSequence s1;
        EXPECT_CALL(*component, do_focus_previous());
        EXPECT_CALL(*component, do_has_focus())
            .WillOnce(Return(false));
    }

    container.focus_previous();

    ASSERT_FALSE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_two_components, skips_components_that_refuse_focus_previous_on_focus_previous)
{
    {
        InSequence s1;
        EXPECT_CALL(*component1, do_focus_previous());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component0, do_focus_previous());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
    }

    container.focus_previous();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_two_components_where_the_last_has_focus, does_not_yield_focus_on_previous_focus_when_the_component_does_not_yield_focus)
{
    {
        InSequence s1;
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component1, do_focus_previous());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));
    }

    container.focus_previous();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_three_components_where_the_last_has_focus, skips_components_that_refuse_focus_when_the_focused_component_yields_focus_on_previous_focus)
{
    {
        InSequence s1;
        EXPECT_CALL(*component2, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component2, do_focus_previous());
        EXPECT_CALL(*component2, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component1, do_focus_previous());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component0, do_focus_previous());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
    }

    container.focus_previous();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_two_components_where_the_last_has_focus, loses_focus_on_previous_focus_if_no_component_accepts_previous_focus)
{
    {
        InSequence s1;
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component1, do_focus_previous());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component0, do_focus_previous());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(false));
    }

    container.focus_previous();

    ASSERT_FALSE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(1, focus_lost_count);
}
