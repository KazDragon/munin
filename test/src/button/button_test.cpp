#include <munin/button.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/virtual_key.hpp>
#include <gtest/gtest.h>

using testing::ValuesIn;

TEST(a_new_button, can_be_constructed_from_a_string)
{
    auto const size = terminalpp::extent{6, 3};
    
    auto button = munin::make_button(" OK ");
    button->set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_EQ('O', canvas[2][1]);
    ASSERT_EQ('K', canvas[3][1]);
}

TEST(a_new_button, can_be_constructed_from_a_terminal_string)
{
    auto const size = terminalpp::extent{8, 3};
    
    using namespace terminalpp::literals;
    auto button = munin::make_button("CANCEL"_ts);
    button->set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface(canvas);
    
    button->draw(surface, {{}, size});
    
    ASSERT_EQ('A', canvas[2][1]);
    ASSERT_EQ('E', canvas[5][1]);
}

TEST(a_new_button, can_receive_focus)
{
    munin::button button{""};
    button.set_focus();
    ASSERT_TRUE(button.has_focus());
}

TEST(a_new_button, receives_focus_when_clicked)
{
    munin::button button{""};
    button.event(terminalpp::mouse::event{
        terminalpp::mouse::event_type::left_button_down});
    ASSERT_TRUE(button.has_focus());
}

using event_emission_data = std::tuple<
    boost::any, // event
    bool        // emits signal
>;

class a_button :
    public testing::TestWithParam<event_emission_data>
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

TEST_P(a_button, emits_on_click)
{
    auto const &event = std::get<0>(GetParam());
    auto const &click_should_be_received = std::get<1>(GetParam());

    bool click_received = false;
    button_.on_click.connect(
        [&click_received]
        {
            click_received = true;
        });
      
    button_.event(event);
    ASSERT_EQ(click_should_be_received, click_received);
}

INSTANTIATE_TEST_SUITE_P(
    a_button_emits_on_click_for_certain_events,
    a_button,
    ValuesIn({
        event_emission_data{ terminalpp::mouse::event{terminalpp::mouse::event_type::left_button_down}, true },
        event_emission_data{ terminalpp::virtual_key{terminalpp::vk::enter}, true },
        event_emission_data{ terminalpp::virtual_key{terminalpp::vk::space}, true },
    }));

INSTANTIATE_TEST_SUITE_P(
    a_button_does_not_emit_on_click_for_certain_events,
    a_button,
    ValuesIn({
        event_emission_data{ terminalpp::mouse::event{terminalpp::mouse::event_type::right_button_down}, false },
        event_emission_data{ terminalpp::mouse::event{terminalpp::mouse::event_type::button_up}, false },
        event_emission_data{ terminalpp::virtual_key{terminalpp::vk::slash}, false },
        event_emission_data{ terminalpp::virtual_key{terminalpp::vk::tilde}, false },
    }));
