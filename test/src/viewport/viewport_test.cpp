#include "viewport_test.hpp"
#include "fill_canvas.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/canvas.hpp>
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

INSTANTIATE_TEST_CASE_P(
    viewport_size,
    viewport_preferred_size_test,
    Values
    (
        terminalpp::extent(0, 0),
        terminalpp::extent(1, 1),
        terminalpp::extent(4, 5)
    )
);

namespace 
{

class a_viewport : 
    public a_viewport_with_mock_tracked_component,
    public testing::Test
{
};

}

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
    char const *test_event = "test event";
    boost::any received_event;
    
    EXPECT_CALL(*tracked_component_, do_event(_))
        .WillOnce(Invoke([&received_event](const boost::any& event)
        {
            received_event = event;
        }));

    viewport_->event(test_event);
    
    char const **result = boost::any_cast<char const*>(&received_event);
    ASSERT_TRUE(result != nullptr);
    ASSERT_TRUE(*result != nullptr);
    ASSERT_STREQ(test_event, *result);
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
