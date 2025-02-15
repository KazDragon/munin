#include "redraw.hpp"

#include <gmock/gmock.h>
#include <munin/render_surface.hpp>
#include <munin/toggle_button.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

using testing::ElementsAreArray;
using testing::ValuesIn;

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
        [&toggle_state](bool state) { toggle_state = state; });

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
        [&toggle_state](bool state) { toggle_state = state; });

    button->set_toggle_state(false);

    ASSERT_FALSE(toggle_state);
}

TEST(
    setting_the_toggle_state_to_the_current_state,
    does_not_fire_a_state_changed_event)
{
    auto const size = terminalpp::extent{3, 3};

    auto button = munin::make_toggle_button(true);
    button->set_size(size);

    bool on_state_changed_event_fired = false;
    button->on_state_changed.connect([&on_state_changed_event_fired](bool) {
        on_state_changed_event_fired = true;
    });

    button->set_toggle_state(true);

    ASSERT_FALSE(on_state_changed_event_fired);
}

TEST(setting_the_toggle_state_of_a_toggle_button, fires_a_redraw_request)
{
    auto const size = terminalpp::extent{3, 3};

    auto button = munin::make_toggle_button();
    button->set_size(size);

    std::vector<terminalpp::rectangle> redraw_regions;
    button->on_redraw.connect(append_regions_to_container(redraw_regions));

    button->set_toggle_state(true);

    ASSERT_THAT(
        redraw_regions,
        ElementsAreArray({
            terminalpp::rectangle{{1, 1}, {1, 1}}
    }));
}

namespace {

using event_emission_data = std::tuple<
    bool,      // checked status
    std::any,  // event
    bool,      // emits signal
    bool       // end status
    >;

class a_toggle_button : public testing::TestWithParam<event_emission_data>
{
public:
    a_toggle_button()
    {
        toggle_button_.set_size({3, 3});
    }

protected:
    munin::toggle_button toggle_button_;
};

}  // namespace

TEST_P(a_toggle_button, emits_on_state_changed)
{
    auto const &begin_state = std::get<0>(GetParam());
    auto const &event = std::get<1>(GetParam());
    auto const &state_changed_should_be_received = std::get<2>(GetParam());
    auto const &expected_end_state = std::get<3>(GetParam());

    toggle_button_.set_toggle_state(begin_state);

    bool state_changed_received = false;
    bool end_state = begin_state;
    toggle_button_.on_state_changed.connect([&](bool new_state) {
        state_changed_received = true;
        end_state = new_state;
    });

    toggle_button_.event(event);
    ASSERT_EQ(state_changed_should_be_received, state_changed_received);
    ASSERT_EQ(expected_end_state, end_state);
}

INSTANTIATE_TEST_SUITE_P(
    a_toggle_button_emits_on_state_changed_for_certain_events,
    a_toggle_button,
    ValuesIn({
        event_emission_data{
                            false, terminalpp::mouse::event{
                terminalpp::mouse::event_type::left_button_down},
                            true,                                                                         true },
        event_emission_data{
                            false, terminalpp::virtual_key{terminalpp::vk::enter},                  true, true },
        event_emission_data{
                            false, terminalpp::virtual_key{terminalpp::vk::space},                  true, true },
        event_emission_data{
                            true,  terminalpp::mouse::event{
                terminalpp::mouse::event_type::left_button_down},
                            true,                                                                         false},
        event_emission_data{
                            true,  terminalpp::virtual_key{terminalpp::vk::enter},                  true, false},
        event_emission_data{
                            true,  terminalpp::virtual_key{terminalpp::vk::space},                  true, false},
}));

INSTANTIATE_TEST_SUITE_P(
    a_toggle_button_does_not_emit_on_click_for_certain_events,
    a_toggle_button,
    ValuesIn({
        event_emission_data{
                            false, terminalpp::mouse::event{
                terminalpp::mouse::event_type::right_button_down},
                            false,                                                                            false},
        event_emission_data{
                            false, terminalpp::mouse::event{terminalpp::mouse::event_type::button_up},
                            false,                                                                            false},
        event_emission_data{
                            false, terminalpp::virtual_key{terminalpp::vk::slash},
                            false,                                                                            false},
        event_emission_data{
                            false, terminalpp::virtual_key{terminalpp::vk::tilde},
                            false,                                                                            false},

        event_emission_data{
                            true,  terminalpp::mouse::event{
                terminalpp::mouse::event_type::right_button_down},
                            false,                                                                            true },
        event_emission_data{
                            true,  terminalpp::mouse::event{terminalpp::mouse::event_type::button_up},
                            false,                                                                            true },
        event_emission_data{
                            true,  terminalpp::virtual_key{terminalpp::vk::slash},                     false, true },
        event_emission_data{
                            true,  terminalpp::virtual_key{terminalpp::vk::tilde},                     false, true },
}));
