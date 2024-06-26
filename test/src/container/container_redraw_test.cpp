#include "container_test.hpp"
#include "redraw.hpp"

using testing::Return;

TEST_F(
    a_container_with_one_component,
    requests_redraws_relative_to_component_position_when_subcomponent_requests_redraws)
{
    static auto const redraw_region = terminalpp::rectangle{
        {0,  0},
        {10, 5}
    };
    static auto const component_position = terminalpp::point{2, 3};

    container_.on_redraw.connect([](auto const &redraw_regions) {
        static auto const expected_redraw_region = terminalpp::rectangle{
            {2,  3},
            {10, 5}
        };
        assert_equivalent_redraw_regions(
            redraw_regions, {expected_redraw_region});
    });

    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(component_position));

    component_->on_redraw({redraw_region});

    ASSERT_EQ(1, redraw_count_);
}

TEST_F(
    a_container_with_one_component,
    does_not_emit_redraw_event_when_removed_component_emits_redraw_event)
{
    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(0, 0)));

    container_.remove_component(component_);

    component_->on_redraw({});

    ASSERT_EQ(0, redraw_count_);
}
