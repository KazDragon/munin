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

TEST(a_toggle_button_constructed_with_a_toggle_state, shows_the_toggle_state)
{
    auto const size = terminalpp::extent{3, 3};
    
    auto button = munin::make_toggle_button(true);
    button->set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_NE(' ', canvas[1][1]);
}