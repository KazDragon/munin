#include "container_test.hpp"

#include <munin/event_context.hpp>
#include <munin/mouse_event.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <tuple>
#include <vector>

using testing::_;
using testing::InSequence;
using testing::Return;
using testing::ValuesIn;

namespace {

auto keypress(terminalpp::vk key) -> terminalpp::virtual_key
{
    return terminalpp::virtual_key{
        key, terminalpp::vk_modifier::none, 1, terminalpp::byte{'\t'}};
}

auto tab_key() -> terminalpp::virtual_key
{
    return keypress(terminalpp::vk::ht);
}

auto back_tab_key() -> terminalpp::virtual_key
{
    return keypress(terminalpp::vk::bt);
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

}  // namespace

TEST_F(
    a_container_with_one_component, does_not_forward_events_to_the_subcomponent)
{
    // Because the component does not have focus, it does not make sense for
    // the component to receive the events.
    EXPECT_CALL(*component_, do_has_focus()).WillRepeatedly(Return(false));

    container_.event('X');
}

TEST_F(
    a_container_with_one_component_that_has_focus,
    forwards_events_to_the_subcomponent)
{
    EXPECT_CALL(*component_, do_has_focus()).WillOnce(Return(true));

    EXPECT_CALL(*component_, do_event(_)).WillOnce([](std::any const &event) {
        char const *p = std::any_cast<char>(&event);
        ASSERT_NE(nullptr, p);
        ASSERT_EQ('X', *p);
    });
    container_.event('X');
}

TEST_F(
    a_container,
    forwards_context_aware_common_events_to_the_focused_subcomponent)
{
    auto component = std::make_shared<context_observing_component>();
    container_.add_component(component);

    EXPECT_CALL(*component, do_has_focus()).WillOnce(Return(true));

    munin::event_context context;
    container_.event('X', context);

    char const *p = std::any_cast<char>(&component->event_);
    ASSERT_NE(nullptr, p);
    ASSERT_EQ('X', *p);
    ASSERT_EQ(&context, component->context_);
}

TEST_F(
    a_container_with_two_components_where_the_last_has_focus,
    skips_the_first_and_forwards_events_to_the_last_subcomponent)
{
    EXPECT_CALL(*component0_, do_has_focus()).WillOnce(Return(false));

    EXPECT_CALL(*component1_, do_has_focus()).WillOnce(Return(true));

    EXPECT_CALL(*component1_, do_event(_)).WillOnce([](std::any const &event) {
        char const *p = std::any_cast<char>(&event);
        ASSERT_NE(nullptr, p);
        ASSERT_EQ('X', *p);
    });
    container_.event('X');
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    tab_moves_focus_to_the_next_subcomponent)
{
    {
        InSequence s1;
        EXPECT_CALL(*component0_, do_has_focus()).WillOnce(Return(true));
        EXPECT_CALL(*component0_, do_focus_next())
            .WillOnce(std::ref(component0_->on_focus_lost));
        EXPECT_CALL(*component0_, do_has_focus()).WillOnce(Return(false));

        EXPECT_CALL(*component1_, do_focus_next())
            .WillOnce(std::ref(component1_->on_focus_set));
        EXPECT_CALL(*component1_, do_has_focus()).WillOnce(Return(true));
    }

    container_.event(tab_key());

    ASSERT_TRUE(container_.has_focus());
}

TEST_F(
    a_container_with_two_components_where_the_last_has_focus,
    back_tab_moves_focus_to_the_previous_subcomponent)
{
    {
        InSequence s1;
        EXPECT_CALL(*component1_, do_has_focus()).WillOnce(Return(true));
        EXPECT_CALL(*component1_, do_focus_previous())
            .WillOnce(std::ref(component1_->on_focus_lost));
        EXPECT_CALL(*component1_, do_has_focus()).WillOnce(Return(false));

        EXPECT_CALL(*component0_, do_focus_previous())
            .WillOnce(std::ref(component0_->on_focus_set));
        EXPECT_CALL(*component0_, do_has_focus()).WillOnce(Return(true));
    }

    container_.event(back_tab_key());

    ASSERT_TRUE(container_.has_focus());
}

TEST_F(
    a_container_with_one_component,
    forwards_mouse_events_even_though_the_component_has_no_focus)
{
    static terminalpp::mouse::event const ev{
        terminalpp::mouse::event_type::left_button_down, {0, 0}
    };

    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(0, 0)));

    EXPECT_CALL(*component_, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component_, do_event(_)).WillOnce([](std::any const &event) {
        auto const *p = std::any_cast<terminalpp::mouse::event>(&event);
        ASSERT_NE(nullptr, p);
        ASSERT_EQ(ev, *p);
    });

    container_.event(ev);
}

TEST_F(
    a_container_with_one_component,
    forwards_derived_mouse_events_even_though_the_component_has_no_focus)
{
    static auto const ev = munin::mouse_event{
        .action_ = munin::mouse_event_type::button_down,
        .position_ = {5, 6},
        .button_ = terminalpp::mouse::button::left
    };

    static auto const expected_value = munin::mouse_event{
        .action_ = munin::mouse_event_type::button_down,
        .position_ = {2, 2},
        .button_ = terminalpp::mouse::button::left
    };

    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(3, 4)));

    EXPECT_CALL(*component_, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    munin::event_context context;
    EXPECT_CALL(*component_, do_event(_)).WillOnce([](std::any const &event) {
        auto const *p = std::any_cast<munin::mouse_event>(&event);
        ASSERT_NE(nullptr, p);
        ASSERT_EQ(expected_value, *p);
    });

    container_.event(ev, context);
}

using mouse_report_test_data = std::tuple<
    terminalpp::point,          // Position of component
    terminalpp::extent,         // Size of component
    terminalpp::mouse::event,   // Event as received by component
    terminalpp::mouse::event>;  // Expected forwarded event

using containers_forwarding_mouse_events =
    containers_with_a_component<mouse_report_test_data>;

TEST_P(
    containers_forwarding_mouse_events,
    translate_coordinates_relative_to_component_position)
{
    auto const &param = GetParam();
    auto const &component_position = std::get<0>(param);
    auto const &component_size = std::get<1>(param);
    auto const &event = std::get<2>(param);
    auto const &expected_value = std::get<3>(param);

    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(component_position));

    EXPECT_CALL(*component_, do_get_size()).WillOnce(Return(component_size));

    EXPECT_CALL(*component_, do_event(_))
        .WillOnce([&expected_value](std::any event) {
            auto *mouse_event = std::any_cast<terminalpp::mouse::event>(&event);
            ASSERT_NE(nullptr, mouse_event);
            ASSERT_EQ(expected_value, *mouse_event);
        });
    container_.event(event);
}

INSTANTIATE_TEST_SUITE_P(
    test,
    containers_forwarding_mouse_events,
    ValuesIn(std::vector<mouse_report_test_data>{
        mouse_report_test_data{
                               {0, 0},
                               {20, 20},
                               {terminalpp::mouse::event_type::left_button_down, {15, 15}},
                               {terminalpp::mouse::event_type::left_button_down, {15, 15}}},
        mouse_report_test_data{
                               {10, 0},
                               {20, 20},
                               {terminalpp::mouse::event_type::left_button_down, {15, 15}},
                               {terminalpp::mouse::event_type::left_button_down, {5, 15}} },
        mouse_report_test_data{
                               {0, 10},
                               {20, 20},
                               {terminalpp::mouse::event_type::left_button_down, {15, 15}},
                               {terminalpp::mouse::event_type::left_button_down, {15, 5}} },
        mouse_report_test_data{
                               {10, 10},
                               {20, 20},
                               {terminalpp::mouse::event_type::left_button_down, {15, 15}},
                               {terminalpp::mouse::event_type::left_button_down, {5, 5}}  }
}));

TEST_F(
    a_container_with_two_components,
    skips_components_that_are_not_at_the_mouse_report_location)
{
    static terminalpp::mouse::event const event = {
        terminalpp::mouse::event_type::left_button_down, {20, 20}
    };

    static terminalpp::mouse::event const expected_value = {
        terminalpp::mouse::event_type::left_button_down, {5, 5}
    };

    EXPECT_CALL(*component0_, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 0)));

    EXPECT_CALL(*component0_, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component1_, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(15, 15)));

    EXPECT_CALL(*component1_, do_get_size())
        .WillOnce(Return(terminalpp::extent(10, 10)));

    EXPECT_CALL(*component1_, do_event(_)).WillOnce([](std::any event) {
        auto *report = std::any_cast<terminalpp::mouse::event>(&event);
        ASSERT_NE(nullptr, report);
        ASSERT_EQ(expected_value, *report);
    });

    container_.event(event);
}
