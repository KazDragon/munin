#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mock/component.hpp>
#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>

using testing::_;

TEST(a_component, event_dispatch_passes_context_to_do_event)
{
    struct tag
    {
    };

    mock_component component;
    munin::event_context context;

    EXPECT_CALL(component, do_event(_, _))
        .WillOnce([](std::any const &event, munin::event_context &ctx) {
            ASSERT_NE(nullptr, std::any_cast<tag>(&event));
            ASSERT_NE(nullptr, &ctx);
        });

    component.event(tag{}, context);
}

TEST(a_mouse_event, represents_a_derived_left_button_down)
{
    auto const event = munin::mouse_event{
        .action_ = munin::mouse_event_type::button_down,
        .position_ = {1, 2},
        .button_ = terminalpp::mouse::button::left,
        .modifiers_ = terminalpp::vk_modifier::shift
    };

    ASSERT_EQ(munin::mouse_event_type::button_down, event.action_);
    ASSERT_EQ(terminalpp::point(1, 2), event.position_);
    ASSERT_EQ(terminalpp::mouse::button::left, event.button_);
    ASSERT_EQ(terminalpp::vk_modifier::shift, event.modifiers_);
}
