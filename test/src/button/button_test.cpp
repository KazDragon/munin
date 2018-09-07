#include <munin/button.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/ansi/mouse.hpp>
#include <gtest/gtest.h>

TEST(a_new_button, can_be_constructed_from_a_string)
{
    auto const size = terminalpp::extent{6, 3};
    
    munin::button button(" OK ");
    button.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button.draw(surface, {{}, size});
    
    ASSERT_EQ('O', canvas[2][1]);
    ASSERT_EQ('K', canvas[3][1]);
}

TEST(a_new_button, can_be_constructed_from_a_terminal_string)
{
    auto const size = terminalpp::extent{8, 3};
    
    using namespace terminalpp::literals;
    munin::button button("CANCEL"_ts);
    button.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button.draw(surface, {{}, size});
    
    ASSERT_EQ('A', canvas[2][1]);
    ASSERT_EQ('E', canvas[5][1]);
}

class a_button : public testing::Test
{
public :
    a_button()
      : button_(" OK ")
    {
        button_.set_size({6, 3});
    }
    
protected :
    munin::button button_;
};

TEST_F(a_button, emits_an_on_click_signal_when_a_mouse_down_event_is_received)
{
    bool click_received = false;
    button_.on_click.connect(
        [&click_received]
        {
            click_received = true;
        });
        
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.button_ = terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN;
    
    button_.event(mouse_event);
    
    ASSERT_TRUE(click_received);
}

TEST_F(a_button, does_not_emit_an_on_click_signal_when_a_mouse_up_event_is_received)
{
    bool click_received = false;
    button_.on_click.connect(
        [&click_received]
        {
            click_received = true;
        });
        
    terminalpp::ansi::mouse::report mouse_event;
    mouse_event.button_ = terminalpp::ansi::mouse::report::BUTTON_UP;
    
    button_.event(mouse_event);
    
    ASSERT_FALSE(click_received);
}
