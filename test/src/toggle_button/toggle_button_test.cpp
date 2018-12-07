#include <munin/render_surface.hpp>
#include <munin/toggle_button.hpp>
#include <terminalpp/canvas.hpp>
#include <gtest/gtest.h>

TEST(a_default_constructed_toggle_button, shows_an_unchecked_state)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button();
    button->set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_EQ(' ', canvas[1][1]);
}

TEST(a_toggle_button_constructed_with_a_toggle_state, shows_a_checked_state)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(true);
    button->set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_NE(' ', canvas[1][1]);
}

TEST(setting_the_toggle_state_of_a_toggle_button, shows_a_checked_state)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(false);
    button->set_size(size);
    button->set_toggle_state(true);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_NE(' ', canvas[1][1]);
}

TEST(clearing_the_toggle_state_of_a_toggle_button, shows_an_unchecked_state)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(true);
    button->set_size(size);
    button->set_toggle_state(false);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_EQ(' ', canvas[1][1]);
}

TEST(setting_the_toggle_state_of_a_toggle_button, fires_a_state_changed_event)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button();
    button->set_size(size);
    
    
    bool toggle_state = false;
    button->on_state_changed.connect(
        [&toggle_state](bool state)
        {
            toggle_state = state;
        });
    
    button->set_toggle_state(true);
    
    ASSERT_TRUE(toggle_state);
}

TEST(clearing_the_toggle_state_of_a_toggle_button, fires_a_state_changed_event)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(true);
    button->set_size(size);
    
    
    bool toggle_state = true;
    button->on_state_changed.connect(
        [&toggle_state](bool state)
        {
            toggle_state = state;
        });
    
    button->set_toggle_state(false);
    
    ASSERT_FALSE(toggle_state);
}

TEST(setting_the_toggle_state_to_the_current_state, does_not_fire_a_state_changed_event)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(true);
    button->set_size(size);
    
    
    bool on_state_changed_event_fired = false;
    button->on_state_changed.connect(
        [&on_state_changed_event_fired](bool)
        {
            on_state_changed_event_fired = true;
        });
    
    button->set_toggle_state(true);
    
    ASSERT_FALSE(on_state_changed_event_fired);
}
