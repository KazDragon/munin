#include "window_test.hpp"

#include <gtest/gtest.h>
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
