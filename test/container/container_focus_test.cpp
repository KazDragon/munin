#include <munin/container.hpp>
#include <munin/context.hpp>
#include "container_test.hpp"
#include "../mock/component.hpp"
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;
using testing::Return;
using testing::StrictMock;

TEST_F(a_container, when_focus_is_set_gives_focus_to_the_first_component)
{
    auto component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component);

    EXPECT_CALL(*component, do_is_enabled())
        .WillOnce(Return(true));
    EXPECT_CALL(*component, do_set_focus());

    container_.set_focus();

    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_container, when_focus_is_set_skips_over_disabled_components)
{
    auto disabled_component = std::make_shared<StrictMock<mock_component>>();
    auto enabled_component  = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(disabled_component);
    container_.add_component(enabled_component);

    EXPECT_CALL(*disabled_component, do_is_enabled())
        .WillOnce(Return(false));
    EXPECT_CALL(*enabled_component, do_is_enabled())
        .WillOnce(Return(true));
    EXPECT_CALL(*enabled_component, do_set_focus());

    container_.set_focus();

    ASSERT_EQ(1, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}
