#include "container_test.hpp"

using testing::InSequence;
using testing::Invoke;
using testing::Return;

TEST_F(a_container_with_one_component, sets_focus_when_component_sets_focus)
{
    component->on_focus_set();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(1, focus_set_count);
}

TEST_F(a_container_with_one_component_that_has_focus, loses_focus_when_component_loses_focus)
{
    component->on_focus_lost();

    ASSERT_FALSE(container.has_focus());
    ASSERT_EQ(1, focus_lost_count);
}

TEST_F(a_container_with_two_components_where_the_first_has_focus, removes_focus_from_the_first_component_when_the_second_component_sets_focus)
{
    EXPECT_CALL(*component0, do_has_focus())
        .WillOnce(Return(true));
    EXPECT_CALL(*component0, do_lose_focus())
        .WillOnce(Invoke(std::ref(component0->on_focus_lost)));

    component1->on_focus_set();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_two_components_where_the_last_has_focus, removes_focus_from_the_second_component_when_the_first_component_sets_focus)
{
    EXPECT_CALL(*component1, do_has_focus())
        .WillOnce(Return(true));
    EXPECT_CALL(*component1, do_lose_focus())
        .WillOnce(Invoke(std::ref(component1->on_focus_lost)));

    component0->on_focus_set();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}
