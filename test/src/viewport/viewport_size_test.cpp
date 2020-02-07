#include "viewport_test.hpp"
#include <munin/viewport.hpp>
#include <gtest/gtest.h>

using testing::ValuesIn;

namespace {
    
using viewport_size_test_data = std::tuple<
    terminalpp::extent, // component's preferred size
    terminalpp::point,  // initial tracked component cursor position
    terminalpp::extent, // changed viewport size
    terminalpp::point   // expected viewport cursor position
>;

using viewport_size_test = 
    viewport_mock_test_with_data<viewport_size_test_data>;

}

TEST_P(viewport_size_test, viewports_track_size_changes)
{
    using std::get;
    using testing::Return;
    
    auto const &param = GetParam();
    auto const &tracked_component_preferred_size = get<0>(param);
    auto const &tracked_component_cursor_position = get<1>(param);
    auto const &changed_viewport_size = get<2>(param);
    auto const &expected_viewport_cursor_position = get<3>(param);
    
    // Set up the conditions of the tracked component
    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_component_preferred_size));
    tracked_component_->on_preferred_size_changed();
    tracked_component_->set_cursor_position(tracked_component_cursor_position);
    
    // Change the size of the viewport.  This may cause the viewport's
    // cursor position to change, though the tracked component's cursor
    // position must not.
    auto const old_tracked_cursor_position = 
        tracked_component_->get_cursor_position();
    viewport_->set_size(changed_viewport_size);
    auto const new_tracked_cursor_position =
        tracked_component_->get_cursor_position();
        
    ASSERT_EQ(old_tracked_cursor_position, new_tracked_cursor_position);
    ASSERT_EQ(expected_viewport_cursor_position, viewport_->get_cursor_position());
}

INSTANTIATE_TEST_CASE_P(
    changing_viewport_size,
    viewport_size_test,
    ValuesIn({
        // Base case: everything stays the same.
        viewport_size_test_data{
            {6, 6}, {1, 1}, {3, 3}, {1, 1}
        },
        
        // Extending the viewport eastward when at the origin does not
        // move the cursor.
        viewport_size_test_data{
            {6, 6}, {1, 1}, {6, 3}, {1, 1}
        },
        
        // Extending the viewport southward when at the origin does not
        // move the cursor.
        viewport_size_test_data{
            {6, 6}, {1, 1}, {3, 6}, {1, 1}
        },
        
        // Shrinking the viewport so that the cursor is still in view does
        // not move the cursor.
        viewport_size_test_data{
            {6, 6}, {1, 1}, {2, 2}, {1, 1}
        },
        
    })
);