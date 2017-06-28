#include <munin/container.hpp>
#include <munin/context.hpp>
#include "container_test.hpp"
#include "../mock/component.hpp"
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;
using testing::InSequence;
using testing::Return;
using testing::StrictMock;

TEST_F(a_container, with_no_focus_calls_focus_next_on_subcomponent_on_focus_next)
{
    auto component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component);

    EXPECT_CALL(*component, do_focus_next());
    EXPECT_CALL(*component, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, with_no_focusable_subcomponents_refuses_focus_on_focus_next)
{
    auto component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component);

    EXPECT_CALL(*component, do_focus_next());
    EXPECT_CALL(*component, do_has_focus())
        .WillOnce(Return(false));

    container_.focus_next();

    ASSERT_FALSE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, skips_subcomponents_that_refuse_focus_on_focus_next)
{
    auto unfocusable_component = std::make_shared<StrictMock<mock_component>>();
    auto focusable_component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(unfocusable_component);
    container_.add_component(focusable_component);

    EXPECT_CALL(*unfocusable_component, do_focus_next());
    EXPECT_CALL(*unfocusable_component, do_has_focus())
        .WillOnce(Return(false));

    EXPECT_CALL(*focusable_component, do_focus_next());
    EXPECT_CALL(*focusable_component, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, with_a_focused_component_that_does_not_yield_focus_on_next_focus_does_not_change_focus)
{
    auto component0 = std::make_shared<StrictMock<mock_component>>();
    auto component1 = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component0);
    container_.add_component(component1);

    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    focus_set_count = 0;

    {
        InSequence s1;
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component0, do_focus_next());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
    }

    container_.focus_next();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, with_a_focused_component_that_yields_focus_on_next_focus_focuses_the_next_component)
{
    auto component0 = std::make_shared<StrictMock<mock_component>>();
    auto component1 = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component0);
    container_.add_component(component1);

    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    focus_set_count = 0;

    {
        InSequence s1;
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component0, do_focus_next());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component1, do_focus_next());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));
    }

    container_.focus_next();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, with_a_focused_component_that_yields_focus_on_next_focus_skips_unfocusable_components_when_focusing_next_component)
{
    auto component0 = std::make_shared<StrictMock<mock_component>>();
    auto unfocusable_component = std::make_shared<StrictMock<mock_component>>();
    auto component1 = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component0);
    container_.add_component(unfocusable_component);
    container_.add_component(component1);

    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    focus_set_count = 0;

    {
        InSequence s1;
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*component0, do_focus_next());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*unfocusable_component, do_focus_next());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*component1, do_focus_next());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));
    }

    container_.focus_next();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, with_a_focused_component_that_yields_focus_on_next_focus_loses_focus_if_no_component_accepts_next_focus)
{
    auto focusable_component = std::make_shared<StrictMock<mock_component>>();
    auto unfocusable_component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(focusable_component);
    container_.add_component(unfocusable_component);

    EXPECT_CALL(*focusable_component, do_focus_next());
    EXPECT_CALL(*focusable_component, do_has_focus())
        .WillOnce(Return(true));

    container_.focus_next();

    focus_set_count = 0;

    {
        InSequence s1;
        EXPECT_CALL(*focusable_component, do_has_focus())
            .WillOnce(Return(true));
        EXPECT_CALL(*focusable_component, do_focus_next());
        EXPECT_CALL(*focusable_component, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*unfocusable_component, do_focus_next());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));
    }

    container_.focus_next();

    ASSERT_FALSE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(1, focus_lost_count);
}
