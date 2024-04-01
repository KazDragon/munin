#include "assert_similar.hpp"
#include "fill_canvas.hpp"

#include <gtest/gtest.h>
#include <munin/button.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

using namespace terminalpp::literals;  // NOLINT
using testing::ValuesIn;

TEST(a_new_button, can_be_constructed_from_a_string)
{
    static auto const button_size = terminalpp::extent{6, 3};
    static auto const canvas_size = terminalpp::extent{8, 5};

    auto button = munin::make_button(" OK ");
    button->set_size(button_size);

    terminalpp::canvas canvas(canvas_size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface(canvas);
    surface.offset_by({1, 1});
    button->draw(surface);

    assert_similar_canvas_block(
        {
            // clang-format off
          R"(XXXXXXXX)"_ts,
          R"(X\U256D\U2500\U2500\U2500\U2500\U256EX)"_ets,
          R"(X\U2502 OK \U2502X)"_ets,
          R"(X\U2570\U2500\U2500\U2500\U2500\U256FX)"_ets,
          R"(XXXXXXXX)"_ts,
            // clang-format on
        },
        canvas);
}

TEST(a_new_button, can_be_constructed_from_a_terminal_string)
{
    auto const button_size = terminalpp::extent{8, 3};
    auto const canvas_size = terminalpp::extent{10, 5};

    auto button = munin::make_button("CANCEL"_ts);
    button->set_size(button_size);

    terminalpp::canvas canvas(canvas_size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface(canvas);
    surface.offset_by({1, 1});

    button->draw(surface);

    assert_similar_canvas_block(
        {
            // clang-format off
          R"(XXXXXXXXXX)"_ts,
          R"(X\U256D\U2500\U2500\U2500\U2500\U2500\U2500\U256EX)"_ets,
          R"(X\U2502CANCEL\U2502X)"_ets,
          R"(X\U2570\U2500\U2500\U2500\U2500\U2500\U2500\U256FX)"_ets,
          R"(XXXXXXXXXX)"_ts,
            // clang-format on
        },
        canvas);
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
    boost::any,  // event
    bool         // emits signal
    >;

class a_button : public testing::TestWithParam<event_emission_data>
{
public:
    a_button() : button_(" OK ")
    {
        button_.set_size({6, 3});
    }

protected:
    munin::button button_;
};

TEST_P(a_button, emits_on_click)
{
    auto const &event = std::get<0>(GetParam());
    auto const &click_should_be_received = std::get<1>(GetParam());

    bool click_received = false;
    button_.on_click.connect([&click_received] { click_received = true; });

    button_.event(event);
    ASSERT_EQ(click_should_be_received, click_received);
}

INSTANTIATE_TEST_SUITE_P(
    a_button_emits_on_click_for_certain_events,
    a_button,
    ValuesIn({
        event_emission_data{
                            terminalpp::mouse::event{
                terminalpp::mouse::event_type::left_button_down},
                            true                                                },
        event_emission_data{
                            terminalpp::virtual_key{terminalpp::vk::enter}, true},
        event_emission_data{
                            terminalpp::virtual_key{terminalpp::vk::space}, true},
}));

INSTANTIATE_TEST_SUITE_P(
    a_button_does_not_emit_on_click_for_certain_events,
    a_button,
    ValuesIn({
        event_emission_data{
                            terminalpp::mouse::event{
                terminalpp::mouse::event_type::right_button_down},
                            false                                                                    },
        event_emission_data{
                            terminalpp::mouse::event{terminalpp::mouse::event_type::button_up},
                            false                                                                    },
        event_emission_data{
                            terminalpp::virtual_key{terminalpp::vk::slash},                     false},
        event_emission_data{
                            terminalpp::virtual_key{terminalpp::vk::tilde},                     false},
}));
