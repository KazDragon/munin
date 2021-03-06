#include "viewport_test.hpp"
#include "fill_canvas.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/mouse.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Invoke;
using testing::Return;
using testing::Values;
using testing::_;

namespace {
    
class a_new_viewport : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_{munin::make_edit()};
    std::shared_ptr<munin::viewport> viewport_{munin::make_viewport(edit_)};
};

}

TEST_F(a_new_viewport, is_a_component)
{
    std::shared_ptr<munin::component> comp = viewport_;
}

TEST_F(a_new_viewport, of_zero_size_draws_nothing)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    viewport_->set_position({1, 1});
    viewport_->set_size({0, 0});
    
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    viewport_->draw(surface, {{}, viewport_->get_size()});
    
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][2]);
}

namespace {

class viewport_preferred_size_test 
  : public a_viewport_with_mock_tracked_component,
    public testing::TestWithParam<terminalpp::extent>
{
};

}

TEST_P(viewport_preferred_size_test, the_preferred_size_of_a_viewport_is_the_same_as_the_preferred_size_of_the_tracked_component)
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
    Values
    (
        terminalpp::extent(0, 0),
        terminalpp::extent(1, 1),
        terminalpp::extent(4, 5)
    )
);

TEST_F(a_viewport, with_a_size_larger_than_the_preferred_size_of_the_tracked_component_sets_the_tracked_component_to_the_larger_size)
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

TEST_F(a_viewport, with_a_size_smaller_than_the_tracked_component_allows_the_tracked_component_its_preferred_size)
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
    std::string const test_event = "test event";
    boost::any received_event;
    
    EXPECT_CALL(*tracked_component_, do_event(_))
        .WillOnce(Invoke([&received_event](const boost::any& event)
        {
            received_event = event;
        }));

    viewport_->event(test_event);
    
    auto const *result = boost::any_cast<std::string>(&received_event);
    ASSERT_TRUE(result != nullptr);
    ASSERT_EQ(test_event, *result);
}

TEST_F(a_viewport, forwards_repaint_events_from_the_tracked_component)
{
    auto const preferred_size = terminalpp::extent{3, 3};
    auto const viewport_size  = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_regions.insert(
                redraw_regions.end(), 
                regions.begin(),
                regions.end());
        });

    auto const tracked_redraw_region = terminalpp::rectangle{{}, {3, 3}};
    auto const expected_redraw_region = terminalpp::rectangle{{}, {3, 3}};

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1u, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(a_viewport, clips_repaint_events_that_extend_out_of_the_visible_area)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size  = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_regions.insert(
                redraw_regions.end(), 
                regions.begin(),
                regions.end());
        });

    auto const tracked_redraw_region = terminalpp::rectangle{{}, {4, 4}};
    auto const expected_redraw_region = terminalpp::rectangle{{}, {3, 3}};

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1u, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(a_viewport, discards_repaint_events_that_are_not_in_the_visible_area)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size  = terminalpp::extent{3, 3};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
    tracked_component_->on_preferred_size_changed();
    viewport_->set_size({3, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    viewport_->on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_regions.insert(
                redraw_regions.end(), 
                regions.begin(),
                regions.end());
        });

    auto const tracked_redraw_region = terminalpp::rectangle{{3, 0}, {1, 3}};

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(0u, redraw_regions.size());
}

TEST_F(a_viewport, translates_repaint_events_when_the_tracked_component_is_offset)
{
    auto const preferred_size = terminalpp::extent{5, 5};
    auto const viewport_size  = terminalpp::extent{3, 3};

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
    viewport_->on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_regions.insert(
                redraw_regions.end(), 
                regions.begin(),
                regions.end());
        });

    auto const tracked_redraw_region = terminalpp::rectangle{{2, 2}, {3, 3}};
    auto const expected_redraw_region = terminalpp::rectangle{{}, {3, 3}};

    tracked_component_->on_redraw({tracked_redraw_region});

    ASSERT_EQ(1u, redraw_regions.size());
    ASSERT_EQ(expected_redraw_region, redraw_regions[0]);
}

TEST_F(a_viewport, has_a_negative_cursor_state_when_the_tracked_component_has_a_negative_cursor_state)
{
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(false));

    ASSERT_FALSE(viewport_->get_cursor_state());
}

TEST_F(a_viewport, has_a_positive_cursor_state_when_the_tracked_component_has_a_positive_cursor_state)
{
    ON_CALL(*tracked_component_, do_get_cursor_state())
        .WillByDefault(Return(true));

    ASSERT_TRUE(viewport_->get_cursor_state());
}

TEST_F(a_viewport, forwards_cursor_state_change_events_from_the_tracked_component)
{
    bool called = false;
    viewport_->on_cursor_state_changed.connect([&called] { called = true; });

    tracked_component_->on_cursor_state_changed();

    ASSERT_TRUE(called);
}

TEST_F(a_viewport, draws_offset_area_when_viewport_position_is_offset)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');

    ON_CALL(*tracked_component_, do_draw(_, _))
        .WillByDefault(Invoke(
            [](munin::render_surface& surface, 
               terminalpp::rectangle const &region)
            {
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
                       terminalpp::coordinate_type row)
                    {
                        elem = ('a' + column + (row * 4));
                    });
            }
        ));
    
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
    
    ASSERT_EQ(terminalpp::element{'f'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'g'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'h'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'j'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{'k'}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{'l'}, cvs[2][1]);
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
    auto const viewport_mouse_event = terminalpp::mouse::event {
        terminalpp::mouse::event_type::left_button_down,
        { 1, 1 }
    };

    // We expect that the result is that the click will be translated to
    // what the middle of that viewable area is on the tracked component.
    auto const expected_mouse_event = terminalpp::mouse::event {
        terminalpp::mouse::event_type::left_button_down,
        { 3, 3 }
    };

    boost::optional<terminalpp::mouse::event> received_mouse_event;

    ON_CALL(*tracked_component_, do_event(_))
        .WillByDefault(Invoke(
            [&received_mouse_event](boost::any const &ev)
            {
                auto *mouse_event = 
                    boost::any_cast<terminalpp::mouse::event>(&ev);

                if (mouse_event != nullptr)
                {
                    received_mouse_event = *mouse_event;
                }
            }
        ));

    viewport_->event(viewport_mouse_event);

    ASSERT_TRUE(received_mouse_event.is_initialized());
    ASSERT_EQ(expected_mouse_event, *received_mouse_event);
}

TEST_F(a_viewport, whose_tracked_component_gains_focus_reports_focus_gained)
{
    bool focus_set = false;
    viewport_->on_focus_set.connect([&focus_set] { focus_set = true; });

    ON_CALL(*tracked_component_, do_has_focus())
        .WillByDefault(Return(true));
    tracked_component_->on_focus_set();

    ASSERT_TRUE(focus_set);
    ASSERT_TRUE(viewport_->has_focus());
}

TEST_F(a_viewport, whose_tracked_component_loses_focus_reports_focus_lost)
{
    bool focus_lost = false;
    viewport_->on_focus_lost.connect([&focus_lost] { focus_lost = true; });

    ON_CALL(*tracked_component_, do_has_focus())
        .WillByDefault(Return(false));
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