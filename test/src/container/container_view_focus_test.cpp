#include <gtest/gtest.h>
#include <munin/compass_layout.hpp>
#include <munin/edit.hpp>
#include <munin/grid_layout.hpp>
#include <munin/scroll_pane.hpp>
#include <munin/view.hpp>

namespace {

TEST(
    a_nested_view_hierarchy_containing_two_scroll_pane_wrapped_edits,
    moves_focus_to_the_second_edit_on_a_single_focus_next)
{
    auto const first_edit = munin::make_edit();
    auto const second_edit = munin::make_edit();
    auto const first_scroll_pane = munin::make_scroll_pane(first_edit);
    auto const second_scroll_pane = munin::make_scroll_pane(second_edit);

    auto const edits_container = munin::view(
        munin::make_grid_layout({1, 2}),
        first_scroll_pane,
        second_scroll_pane);

    auto const page = munin::view(
        munin::make_compass_layout(),
        munin::view(
            munin::make_compass_layout(),
            munin::view(
                munin::make_compass_layout(),
                edits_container,
                munin::compass_layout::heading::centre),
            munin::compass_layout::heading::south),
        munin::compass_layout::heading::centre);

    first_edit->set_focus();

    ASSERT_TRUE(first_edit->has_focus());
    ASSERT_FALSE(second_edit->has_focus());

    page->focus_next();

    ASSERT_FALSE(first_edit->has_focus());
    ASSERT_FALSE(first_scroll_pane->has_focus());
    ASSERT_TRUE(second_scroll_pane->has_focus());
    ASSERT_TRUE(second_edit->has_focus());
}

TEST(
    a_grid_view_containing_two_scroll_pane_wrapped_edits,
    moves_focus_to_the_second_edit_on_a_single_focus_next)
{
    auto const first_edit = munin::make_edit();
    auto const second_edit = munin::make_edit();
    auto const first_scroll_pane = munin::make_scroll_pane(first_edit);
    auto const second_scroll_pane = munin::make_scroll_pane(second_edit);

    auto const edits_container = munin::view(
        munin::make_grid_layout({1, 2}),
        first_scroll_pane,
        second_scroll_pane);

    first_edit->set_focus();

    ASSERT_TRUE(first_edit->has_focus());
    ASSERT_FALSE(second_edit->has_focus());

    edits_container->focus_next();

    ASSERT_FALSE(first_edit->has_focus());
    ASSERT_FALSE(first_scroll_pane->has_focus());
    ASSERT_TRUE(second_scroll_pane->has_focus());
    ASSERT_TRUE(second_edit->has_focus());
}

}  // namespace
