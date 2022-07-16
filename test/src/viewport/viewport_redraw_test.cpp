#include "viewport_test.hpp"
#include <tuple>
#include <vector>

using testing::ValuesIn;

namespace {

using cursor_movement_redraw_test_data = std::tuple<
    std::vector<terminalpp::point>,           // initial cursor movements
    terminalpp::point,                        // cursor movement whose redraw 
                                              // we want to track
    std::optional<terminalpp::rectangle>    // expected redraw region
>;

class viewport_cursor_movement_redraw_test
  : public viewport_mock_test_with_data<cursor_movement_redraw_test_data>
{
};

}

TEST_P(viewport_cursor_movement_redraw_test, cursor_movements)
{
    using std::get;

    auto const &param = GetParam();
    auto const &initial_cursor_movements = get<0>(param);
    auto const &cursor_movement = get<1>(param);
    auto const &expected_redraw_region = get<2>(param);

    // Make a series of initial cursor movements to set up the desired state.
    for (auto const &position : initial_cursor_movements)
    {
        tracked_component_->set_cursor_position(position);
    }

    // Subscribe to the viewport's redraw event so that we can catch whether
    // a redraw occurred.
    std::optional<terminalpp::rectangle> redraw_region;
    viewport_->on_redraw.connect(
        [&](std::vector<terminalpp::rectangle> const &regions)
        {
            ASSERT_EQ(1u, regions.size());
            redraw_region = regions[0];
        });

    // Fire off the cursor movement whose behaviour we are verifying
    tracked_component_->set_cursor_position(cursor_movement);

    // Make the behavioural assertions.
    ASSERT_EQ(expected_redraw_region, redraw_region);
}

INSTANTIATE_TEST_SUITE_P(
    moving_cursors_redraw_viewports,
    viewport_cursor_movement_redraw_test,
    ValuesIn({
        // Default case: moving the cursor to its existing position
        // doesn't redraw the viewport.
        cursor_movement_redraw_test_data{
            {}, {}, {}
        },
        
        // Moving the cursor within the viewed area will not prompt a redraw.
        cursor_movement_redraw_test_data{
            {}, {0, 2}, {}
        },        

        cursor_movement_redraw_test_data{
            {}, {2, 0}, {}
        },        

        cursor_movement_redraw_test_data{
            {}, {2, 2}, {}
        },        
        
        // Moving the cursor just beyond the viewed area will shift the
        // viewport, and prompt a redraw of the entire viewed area.
        cursor_movement_redraw_test_data{
            {}, {3, 0}, terminalpp::rectangle{{0, 0}, {3, 3}}
        },        
        
        cursor_movement_redraw_test_data{
            {}, {0, 3}, terminalpp::rectangle{{0, 0}, {3, 3}}
        },        

        cursor_movement_redraw_test_data{
            {{0, 3}}, {0, 0}, terminalpp::rectangle{{0, 0}, {3, 3}}
        },        

        cursor_movement_redraw_test_data{
            {{3, 0}}, {0, 0}, terminalpp::rectangle{{0, 0}, {3, 3}}
        },        
    })
);

