#include "viewport_test.hpp"

#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "redraw.hpp"

#include <gmock/gmock.h>
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

using namespace terminalpp::literals;  // NOLINT
using testing::_;
using testing::Return;
using testing::ReturnPointee;
using testing::SaveArg;
using testing::Values;

namespace {

class a_new_viewport : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_{munin::make_edit()};
    std::shared_ptr<munin::viewport> viewport_{munin::make_viewport(edit_)};
};

}  // namespace

TEST_F(a_new_viewport, is_a_component)
{
    std::shared_ptr<munin::component> comp = viewport_;
}

TEST_F(a_new_viewport, has_zero_anchor_bounds)
{
    auto const expected_bounds = terminalpp::rectangle{};

    ASSERT_EQ(expected_bounds, viewport_->get_anchor_bounds());
}

TEST_F(a_new_viewport, of_zero_size_draws_nothing)
{
    terminalpp::canvas cvs{
        {4, 3}
    };
    fill_canvas(cvs, 'x');

    viewport_->set_position({1, 1});
    viewport_->set_size({0, 0});

    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    viewport_->draw(surface, {{}, viewport_->get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "xxxx"_ts,
          "xxxx"_ts,
          "xxxx"_ts,
            // clang-format on
        },
        cvs);
}

namespace {

class viewport_preferred_size_test
  : public a_viewport_with_mock_tracked_component,
    public testing::TestWithParam<terminalpp::extent>
{
};

}  // namespace

TEST_P(
    viewport_preferred_size_test,
    the_preferred_size_of_a_viewport_is_the_same_as_the_preferred_size_of_the_tracked_component)
{
    auto const preferred_size = GetParam();

    bool preferred_size_changed = false;
    viewport_->on_preferred_size_changed.connect(
        [&]() { preferred_size_changed = true; });

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));

    tracked_component_->on_preferred_size_changed();

    ASSERT_EQ(preferred_size, viewport_->get_preferred_size());
    ASSERT_TRUE(preferred_size_changed);
}

INSTANTIATE_TEST_SUITE_P(
    viewport_size,
    viewport_preferred_size_test,
    Values(
        terminalpp::extent(0, 0),
        terminalpp::extent(1, 1),
        terminalpp::extent(4, 5)));

TEST_F(
    a_viewport,
    with_a_size_larger_than_the_preferred_size_of_the_tracked_component_sets_the_tracked_component_to_the_larger_size)
{
    auto const viewport_size = terminalpp::extent{5, 5};
    viewport_->set_size(viewport_size);

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{3, 3};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(viewport_size));
        tracked_component_->on_preferred_size_changed();
    }

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{2, 3};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(viewport_size));
        tracked_component_->on_preferred_size_changed();
    }

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{3, 3};
        auto const new_viewport_size = terminalpp::extent{7, 7};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(new_viewport_size));
        viewport_->set_size(new_viewport_size);
    }
}

TEST_F(
    a_viewport,
    with_a_size_smaller_than_the_tracked_component_allows_the_tracked_component_its_preferred_size)
{
    auto const viewport_size = terminalpp::extent{2, 2};
    viewport_->set_size(viewport_size);

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{3, 3};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(preferred_size));

        tracked_component_->on_preferred_size_changed();
    }

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{17, 4};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(preferred_size));

        tracked_component_->on_preferred_size_changed();
    }

    {
        testing::InSequence _;

        auto const preferred_size = terminalpp::extent{17, 4};
        auto const new_viewport_size = terminalpp::extent{3, 3};
        EXPECT_CALL(*tracked_component_, do_get_preferred_size)
            .WillOnce(Return(preferred_size));
        EXPECT_CALL(*tracked_component_, do_set_size(preferred_size));
        viewport_->set_size(new_viewport_size);
    }
}

TEST_F(a_viewport, forwards_events_to_the_tracked_component)
{
    std::any received_event;
    ON_CALL(*tracked_component_, do_event(_))
        .WillByDefault(SaveArg<0>(&received_event));

    std::string const test_event = "test event";
    viewport_->event(test_event);

    auto const *result = std::any_cast<std::string>(&received_event);
    ASSERT_TRUE(result != nullptr);
    ASSERT_EQ(test_event, *result);
}

TEST_F(a_viewport, forwards_keypress_events_to_the_tracked_component)
{
    std::any received_event;
    ON_CALL(*tracked_component_, do_event(_))
        .WillByDefault(SaveArg<0>(&received_event));

    auto const keypress_event = terminalpp::virtual_key{
        terminalpp::vk::lowercase_a, terminalpp::vk_modifier::none, 1};

    viewport_->event(keypress_event);

    auto const *result =
        std::any_cast<terminalpp::virtual_key>(&received_event);
    ASSERT_TRUE(result != nullptr);
    ASSERT_EQ(keypress_event, *result);
}

TEST_F(a_viewport, forwards_repaint_events_from_the_tracked_component)
{
    auto const preferred_size = terminalpp::extent{3, 3};
    auto const viewport_size = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(append_regions_to_container(redraw_regions));

    auto const tracked_redraw_region = terminalpp::rectangle{
        {},
        {3, 3}
    };
    auto const expected_redraw_region = terminalpp::rectangle{
        {},
        {3, 3}
    };

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1U, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(a_viewport, clips_repaint_events_that_extend_out_of_the_visible_area)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(append_regions_to_container(redraw_regions));

    auto const tracked_redraw_region = terminalpp::rectangle{
        {},
        {4, 4}
    };
    auto const expected_redraw_region = terminalpp::rectangle{
        {},
        {3, 3}
    };

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1U, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(a_viewport, discards_repaint_events_that_are_not_in_the_visible_area)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(append_regions_to_container(redraw_regions));

    auto const tracked_redraw_region = terminalpp::rectangle{
        {3, 0},
        {1, 3}
    };

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(0U, redraw_regions.size());
}

TEST_F(
    a_viewport, translates_repaint_events_when_the_tracked_component_is_offset)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    auto const cursor_position = terminalpp::point{4, 4};
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));
    ON_CALL(*tracked_component_, do_get_cursor_position())
        .WillByDefault(Return(cursor_position));
    tracked_component_->on_cursor_position_changed();

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(append_regions_to_container(redraw_regions));

    auto const tracked_redraw_region = terminalpp::rectangle{
        {2, 2},
        {3, 3}
    };
    auto const expected_redraw_region = terminalpp::rectangle{
        {},
        {3, 3}
    };

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1U, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(
    a_viewport,
    has_a_negative_cursor_state_when_the_tracked_component_has_a_negative_cursor_state)
{
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(false));

    ASSERT_FALSE(viewport_->get_cursor_state());
}

TEST_F(
    a_viewport,
    has_a_positive_cursor_state_when_the_tracked_component_has_a_positive_cursor_state)
{
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));

    ASSERT_TRUE(viewport_->get_cursor_state());
}

TEST_F(
    a_viewport, forwards_cursor_state_change_events_from_the_tracked_component)
{
    bool called = false;
    viewport_->on_cursor_state_changed.connect([&called] { called = true; });

    tracked_component_->on_cursor_state_changed();

    ASSERT_TRUE(called);
}

TEST_F(a_viewport, draws_offset_area_when_viewport_position_is_offset)
{
    terminalpp::canvas cvs{
        {4, 3}
    };
    fill_canvas(cvs, 'x');

    ON_CALL(*tracked_component_, do_draw(_, _))
        .WillByDefault([](munin::render_surface &surface,
                          terminalpp::rectangle const &region) {
            // +---+---+---+---+
            // | a | b | c | d |
            // +---+---+---+---+
            // | e | f | g | h |
            // +---+---+---+---+
            // | i | j | k | l |
            // +---+---+---+---+
            terminalpp::for_each_in_region(
                surface,
                region,
                [](terminalpp::element &elem,
                   terminalpp::coordinate_type column,
                   terminalpp::coordinate_type row) {
                    elem = ('a' + column + (row * 4));
                });
        });

    viewport_->set_position({0, 0});
    viewport_->set_size({3, 2});

    auto const cursor_position = terminalpp::point{3, 2};
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));
    ON_CALL(*tracked_component_, do_get_cursor_position())
        .WillByDefault(Return(cursor_position));
    tracked_component_->on_cursor_position_changed();

    munin::render_surface surface{cvs};
    viewport_->draw(surface, {{}, viewport_->get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "fghx"_ts,
          "jklx"_ts,
          "xxxx"_ts,
            // clang-format on
        },
        cvs);
}

TEST_F(a_viewport, translates_mouse_events_to_the_tracked_component)
{
    auto const tracked_component_preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size = terminalpp::extent{3, 3};
    auto const tracked_component_cursor_position = terminalpp::point{4, 4};

    viewport_->set_size(viewport_size);

    ON_CALL(*tracked_component_, do_get_cursor_position())
        .WillByDefault(Return(tracked_component_cursor_position));
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));
    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_component_preferred_size));
    tracked_component_->on_preferred_size_changed();
    tracked_component_->on_cursor_state_changed();
    tracked_component_->on_cursor_position_changed();

    // The mouse is clicked in the middle of the viewable area.
    auto const viewport_mouse_event = terminalpp::mouse::event{
        terminalpp::mouse::event_type::left_button_down, {1, 1}
    };

    // We expect that the result is that the click will be translated to
    // what the middle of that viewable area is on the tracked component.
    auto const expected_mouse_event = terminalpp::mouse::event{
        terminalpp::mouse::event_type::left_button_down, {3, 3}
    };

    std::optional<terminalpp::mouse::event> received_mouse_event;

    ON_CALL(*tracked_component_, do_event(_))
        .WillByDefault([&received_mouse_event](std::any const &ev) {
            if (const auto *mouse_event =
                    std::any_cast<terminalpp::mouse::event>(&ev);
                mouse_event != nullptr)
            {
                received_mouse_event = *mouse_event;
            }
        });

    viewport_->event(viewport_mouse_event);

    ASSERT_TRUE(received_mouse_event.has_value());
    ASSERT_EQ(expected_mouse_event, *received_mouse_event);
}

TEST_F(a_viewport, whose_tracked_component_gains_focus_reports_focus_gained)
{
    bool focus_set = false;
    viewport_->on_focus_set.connect([&focus_set] { focus_set = true; });

    ON_CALL(*tracked_component_, do_has_focus()).WillByDefault(Return(true));
    tracked_component_->on_focus_set();

    ASSERT_TRUE(focus_set);
    ASSERT_TRUE(viewport_->has_focus());
}

TEST_F(a_viewport, whose_tracked_component_loses_focus_reports_focus_lost)
{
    bool focus_lost = false;
    viewport_->on_focus_lost.connect([&focus_lost] { focus_lost = true; });

    ON_CALL(*tracked_component_, do_has_focus()).WillByDefault(Return(false));
    tracked_component_->on_focus_lost();

    ASSERT_TRUE(focus_lost);
    ASSERT_FALSE(viewport_->has_focus());
}

TEST_F(a_viewport, forwards_set_focus_to_the_tracked_component)
{
    EXPECT_CALL(*tracked_component_, do_set_focus());
    viewport_->set_focus();
}

TEST_F(a_viewport, forwards_lose_focus_to_the_tracked_component)
{
    EXPECT_CALL(*tracked_component_, do_lose_focus());
    viewport_->lose_focus();
}

TEST_F(a_viewport, forwards_focus_next_to_the_tracked_component)
{
    EXPECT_CALL(*tracked_component_, do_focus_next());
    viewport_->focus_next();
}

TEST_F(a_viewport, forwards_focus_previous_to_the_tracked_component)
{
    EXPECT_CALL(*tracked_component_, do_focus_previous());
    viewport_->focus_previous();
}

TEST_F(
    a_viewport,
    when_the_same_size_as_the_tracked_component_has_zero_anchor_bounds)
{
    auto const tracked_size = terminalpp::extent{5, 5};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_size));
    tracked_component_->on_preferred_size_changed();

    viewport_->set_size(tracked_size);

    auto const expected_bounds = terminalpp::rectangle{};
    ASSERT_EQ(expected_bounds, viewport_->get_anchor_bounds());
}

TEST_F(
    a_viewport, when_smaller_than_the_tracked_component_extends_possible_bounds)
{
    auto const tracked_size = terminalpp::extent{8, 6};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_size));
    tracked_component_->on_preferred_size_changed();

    auto const viewport_size = terminalpp::extent{4, 4};
    viewport_->set_size(viewport_size);

    // bounds.size_ = number of positions that the viewport could be anchored
    // to, meaning tracked_size - viewport_size in general.
    auto const expected_bounds = terminalpp::rectangle{
        {},
        {4, 2}
    };
    ASSERT_EQ(expected_bounds, viewport_->get_anchor_bounds());
}

TEST_F(
    a_viewport,
    when_the_cursor_is_moved_beyond_the_bounds_of_the_viewport_moves_the_anchor_origin)
{
    auto const tracked_size = terminalpp::extent{8, 6};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_size));
    tracked_component_->on_preferred_size_changed();

    auto const viewport_size = terminalpp::extent{4, 4};
    viewport_->set_size(viewport_size);

    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));
    tracked_component_->on_cursor_state_changed();

    bool anchor_point_changed = false;
    viewport_->on_anchor_bounds_changed.connect(
        [&] { anchor_point_changed = true; });

    ON_CALL(*tracked_component_, do_get_cursor_position())
        .WillByDefault(Return(terminalpp::point{4, 5}));
    tracked_component_->on_cursor_position_changed();

    EXPECT_TRUE(anchor_point_changed);

    // bounds.origin_ = anchor point of the viewport, i.e. position in the
    // tracked component that is shown at the top-left of the viewport.
    // Moving the origin does not affect the bounds.
    auto const expected_bounds = terminalpp::rectangle{
        {1, 2},
        {4, 2}
    };
    ASSERT_EQ(expected_bounds, viewport_->get_anchor_bounds());
}

TEST_F(
    a_viewport,
    when_the_preferred_size_is_changed_so_that_the_anchor_is_too_far_right_reanchors)
{
    // tracked component will be whatever size we tell it.
    auto tracked_size = terminalpp::extent{};
    ON_CALL(*tracked_component_, do_set_size(_))
        .WillByDefault(SaveArg<0>(&tracked_size));
    ON_CALL(*tracked_component_, do_get_size())
        .WillByDefault(ReturnPointee(&tracked_size));

    auto tracked_preferred_size = terminalpp::extent{};
    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(ReturnPointee(&tracked_preferred_size));

    auto tracked_cursor_position = terminalpp::point{};
    ON_CALL(*tracked_component_, do_get_cursor_position())
        .WillByDefault(ReturnPointee(&tracked_cursor_position));
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));

    auto anchor = terminalpp::rectangle{};
    viewport_->on_anchor_bounds_changed.connect(
        [&] { anchor = viewport_->get_anchor_bounds(); });

    auto const viewport_size = terminalpp::extent{12, 8};
    viewport_->set_size(viewport_size);

    // Now make the tracked component bigger.
    tracked_preferred_size = terminalpp::extent{16, 12};
    tracked_component_->on_preferred_size_changed();

    // Move the cursor to the bottom right so that the parts of the
    // tracked component that will fall out of view are visible.
    tracked_cursor_position = {15, 11};
    tracked_component_->on_cursor_position_changed();

    // Move the cursor into the safe space in the view.  This should
    // not reanchor the viewport.
    tracked_cursor_position = {10, 6};
    tracked_component_->on_cursor_position_changed();

    // This is the actual test.  By shrinking the preferred size of the
    // tracked component, this will cause a re-anchor of the viewport, since
    // some of the view is no longer relevant and more display can be pulled
    // in from the left.  However, this also necessitates a cursor change
    // since it will have to move in the same relation as the anchor.
    tracked_preferred_size = terminalpp::extent{14, 10};
    tracked_component_->on_preferred_size_changed();

    ASSERT_EQ(terminalpp::rectangle({2, 2}, {2, 2}), anchor);
    ASSERT_EQ(terminalpp::point(8, 4), viewport_->get_cursor_position());
}
