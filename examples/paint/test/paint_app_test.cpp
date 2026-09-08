#include <gtest/gtest.h>
#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <paint/paint_app.hpp>
#include <paint/paint_model.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <memory>

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

TEST(a_paint_app, forwards_mouse_events_to_the_canvas)
{
    auto model = std::make_shared<paint::paint_model>(terminalpp::extent{0, 0});
    auto app = paint::paint_app{model};
    app.set_size({40, 20});

    munin::event_context context;
    app.event(
        munin::mouse_event{
            .action_ = munin::mouse_event_type::button_down,
            .position_ = {3, 1},
            .button_ = terminalpp::mouse::button::left
    },
        context);

    EXPECT_EQ(model->selected_brush(), model->at({0, 0}));
}
