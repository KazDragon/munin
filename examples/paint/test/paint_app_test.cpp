#include <gtest/gtest.h>
#include <munin/event_context.hpp>
#include <paint/paint_app.hpp>
#include <terminalpp/virtual_key.hpp>

TEST(a_paint_app, emits_quit_for_lowercase_and_uppercase_q)
{
    auto app = paint::paint_app{};
    auto quit_count = 0;
    app.on_quit.connect([&] { ++quit_count; });

    munin::event_context context;
    app.event(
        terminalpp::virtual_key{.key = terminalpp::vk::lowercase_q}, context);
    app.event(
        terminalpp::virtual_key{.key = terminalpp::vk::uppercase_q}, context);

    EXPECT_EQ(2, quit_count);
}

TEST(a_paint_app, factory_returns_composed_content)
{
    auto app = paint::make_paint_app();

    EXPECT_GT(app->get_preferred_size().width_, 0);
    EXPECT_GT(app->get_preferred_size().height_, 0);
}
