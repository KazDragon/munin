#include "window_test.hpp"

#include <gtest/gtest.h>
#include <munin/button.hpp>
#include <munin/container.hpp>
#include <munin/event_context.hpp>

using testing::_;
using testing::SaveArg;

TEST_F(a_window, passes_events_to_the_content)
{
    struct tag
    {
    };

    std::any result;

    EXPECT_CALL(*content_, do_event(_)).WillOnce(SaveArg<0>(&result));

    window_->event(tag{});

    auto *ptag = std::any_cast<tag>(&result);
    ASSERT_NE(nullptr, ptag);
}

class context_observing_component : public mock_component
{
private:
    void do_event(std::any const &ev, munin::event_context &ctx) override
    {
        event_ = ev;
        context_ = &ctx;
    }

public:
    std::any event_;
    munin::event_context *context_ = nullptr;
};

TEST_F(a_window, passes_events_to_the_content_with_context)
{
    struct tag
    {
    };

    auto content = std::make_shared<context_observing_component>();
    auto window = munin::window(terminal_, content);

    window.event(tag{});

    ASSERT_NE(nullptr, std::any_cast<tag>(&content->event_));
    ASSERT_NE(nullptr, content->context_);
}

TEST(
    a_window_mouse_events,
    clicks_a_button_on_left_release_when_clicks_can_be_synthesized)
{
    auto behaviour = terminalpp::behaviour{};
    behaviour.supports_basic_mouse_tracking = true;
    behaviour.supports_sgr_mouse_encoding = true;

    fake_channel channel;
    terminalpp::terminal terminal{channel, behaviour};
    auto button = std::make_shared<munin::button>(" OK ");
    button->set_size({6, 3});
    auto window = munin::window(terminal, button);

    auto click_count = 0;
    button->on_click.connect([&click_count] { ++click_count; });

    window.event(terminalpp::mouse::event{
        .action_ = terminalpp::mouse::event_type::left_button_down,
        .position_ = {1, 1},
        .button_ = terminalpp::mouse::button::left
    });

    ASSERT_EQ(0, click_count);

    window.event(terminalpp::mouse::event{
        .action_ = terminalpp::mouse::event_type::button_up,
        .position_ = {1, 1},
        .button_ = terminalpp::mouse::button::left
    });

    ASSERT_EQ(1, click_count);
}

TEST(
    a_window_mouse_events,
    clicks_a_nested_button_on_left_release_when_clicks_can_be_synthesized)
{
    auto behaviour = terminalpp::behaviour{};
    behaviour.supports_basic_mouse_tracking = true;
    behaviour.supports_sgr_mouse_encoding = true;

    fake_channel channel;
    terminalpp::terminal terminal{channel, behaviour};
    auto container = munin::make_container();
    container->set_size({10, 5});
    auto button = std::make_shared<munin::button>(" OK ");
    button->set_size({6, 3});
    container->add_component(button);
    auto window = munin::window(terminal, container);

    auto click_count = 0;
    button->on_click.connect([&click_count] { ++click_count; });

    window.event(terminalpp::mouse::event{
        .action_ = terminalpp::mouse::event_type::left_button_down,
        .position_ = {1, 1},
        .button_ = terminalpp::mouse::button::left
    });

    ASSERT_EQ(0, click_count);

    window.event(terminalpp::mouse::event{
        .action_ = terminalpp::mouse::event_type::button_up,
        .position_ = {1, 1},
        .button_ = terminalpp::mouse::button::left
    });

    ASSERT_EQ(1, click_count);
}
