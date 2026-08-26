#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mock/component.hpp>
#include <munin/event_context.hpp>

using testing::_;

TEST(a_component, context_aware_event_dispatch_bridges_to_legacy_do_event)
{
    struct tag
    {
    };

    mock_component component;
    munin::event_context context;

    EXPECT_CALL(component, do_event(_)).WillOnce([](std::any const &event) {
        ASSERT_NE(nullptr, std::any_cast<tag>(&event));
    });

    component.event(tag{}, context);
}
