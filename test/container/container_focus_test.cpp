#include "container_test.hpp"

using testing::InSequence;
using testing::Invoke;
using testing::Return;

TEST_F(a_container, when_focus_is_set_gives_focus_to_the_first_component)
{
    auto component = std::make_shared<mock_component>();

    container.add_component(component);

    {
        InSequence s1;
        EXPECT_CALL(*component, do_set_focus())
            .WillOnce(Invoke(std::ref(component->on_focus_set)));
        EXPECT_CALL(*component, do_has_focus())
            .WillOnce(Return(true));
    }

    container.set_focus();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, when_focus_is_set_skips_over_unfocusable_components)
{
    auto unfocusable_component = std::make_shared<mock_component>();
    auto focusable_component  = std::make_shared<mock_component>();

    container.add_component(unfocusable_component);
    container.add_component(focusable_component);

    {
        InSequence s1;
        EXPECT_CALL(*unfocusable_component, do_set_focus());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*focusable_component, do_set_focus())
            .WillOnce(Invoke(std::ref(focusable_component->on_focus_set)));
        EXPECT_CALL(*focusable_component, do_has_focus())
            .WillOnce(Return(true));
    }

    container.set_focus();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, refuses_focus_if_all_components_are_unfocusable)
{
    auto unfocusable_component = std::make_shared<mock_component>();

    container.add_component(unfocusable_component);

    {
        InSequence s1;
        EXPECT_CALL(*unfocusable_component, do_set_focus());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));
    }

    container.set_focus();

    ASSERT_FALSE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container_with_one_component_that_has_focus, loses_focus_from_focused_component_when_focus_is_lost)
{
    EXPECT_CALL(*component, do_has_focus())
        .WillOnce(Return(true));
    EXPECT_CALL(*component, do_lose_focus())
        .WillOnce(Invoke(std::ref(component->on_focus_lost)));

    container.lose_focus();

    ASSERT_FALSE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(1, focus_lost_count);
}

TEST_F(a_container_with_one_component_that_has_focus, does_nothing_when_focus_is_set)
{
    container.set_focus();

    ASSERT_TRUE(container.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}
