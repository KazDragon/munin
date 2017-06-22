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

TEST_F(a_container, when_focus_is_set_gives_focus_to_the_first_component)
{
    auto component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component);

    {
        InSequence s1;
        EXPECT_CALL(*component, do_set_focus());
        EXPECT_CALL(*component, do_has_focus())
            .WillOnce(Return(true));
    }

    container_.set_focus();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, when_focus_is_set_skips_over_unfocusable_components)
{
    auto unfocusable_component = std::make_shared<StrictMock<mock_component>>();
    auto focusable_component  = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(unfocusable_component);
    container_.add_component(focusable_component);

    {
        InSequence s1;
        EXPECT_CALL(*unfocusable_component, do_set_focus());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));

        EXPECT_CALL(*focusable_component, do_set_focus());
        EXPECT_CALL(*focusable_component, do_has_focus())
            .WillOnce(Return(true));
    }

    container_.set_focus();

    ASSERT_TRUE(container_.has_focus());
    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, refuses_focus_if_all_components_are_unfocusable)
{
    auto unfocusable_component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(unfocusable_component);

    {
        InSequence s1;
        EXPECT_CALL(*unfocusable_component, do_set_focus());
        EXPECT_CALL(*unfocusable_component, do_has_focus())
            .WillOnce(Return(false));
    }

    container_.set_focus();

    ASSERT_FALSE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

class a_container_with_focus : public container_test_base
{
protected :
    void SetUp() override
    {
        container_test_base::SetUp();

        container_.add_component(component_);

        {
            InSequence s1;
            EXPECT_CALL(*component_, do_set_focus());
            EXPECT_CALL(*component_, do_has_focus())
                .WillOnce(Return(true));
        }

        container_.set_focus();

        focus_set_count = 0;
    }

    std::shared_ptr<StrictMock<mock_component>> component_ =
        std::make_shared<StrictMock<mock_component>>();
};

TEST_F(a_container_with_focus, loses_focus_from_focused_component_when_focus_is_lost)
{
    EXPECT_CALL(*component_, do_has_focus())
        .WillOnce(Return(true));
    EXPECT_CALL(*component_, do_lose_focus());

    container_.lose_focus();

    ASSERT_FALSE(container_.has_focus());
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(1, focus_lost_count);
}
