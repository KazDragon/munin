#include <munin/container.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include "container_test.hpp"
#include "../mock/component.hpp"
#include "../mock/layout.hpp"
#include <gtest/gtest.h>
#include <iostream>

using testing::StrictMock;
using testing::Return;
using testing::_;

TEST(make_container, makes_a_new_container)
{
    std::shared_ptr<munin::container> container = munin::make_container();
    ASSERT_FALSE(container == nullptr);
}

TEST_F(a_new_container, is_positioned_at_the_origin)
{
    ASSERT_EQ(terminalpp::point(0, 0), container_.get_position());
}

TEST_F(a_new_container, has_empty_size)
{
    ASSERT_EQ(terminalpp::extent(0, 0), container_.get_size());
}

TEST_F(a_new_container, has_empty_preferred_size)
{
    ASSERT_EQ(terminalpp::extent(0, 0), container_.get_preferred_size());
}

TEST_F(a_new_container, is_enabled)
{
    ASSERT_TRUE(container_.is_enabled());
}

TEST_F(a_new_container, can_be_disabled)
{
    container_.disable();
    ASSERT_FALSE(container_.is_enabled());
}

TEST_F(a_new_container, cannot_gain_focus)
{
    container_.set_focus();
    ASSERT_EQ(0, focus_set_count);
}

TEST_F(a_new_container, cannot_lose_focus)
{
    container_.lose_focus();
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_new_container, does_nothing_on_focus_next)
{
    container_.focus_next();
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_new_container, does_nothing_on_focus_previous)
{
    container_.focus_previous();
    ASSERT_EQ(0, focus_set_count);
    ASSERT_EQ(0, focus_lost_count);
}

TEST_F(a_new_container, can_have_its_position_set)
{
    container_.set_position({3, 4});
    ASSERT_EQ(terminalpp::point(3, 4), container_.get_position());
}

TEST_F(a_new_container, can_have_its_size_set)
{
    container_.set_size({5, 6});
    ASSERT_EQ(terminalpp::extent(5, 6), container_.get_size());
}

TEST_F(a_new_container, draws_nothing)
{
    terminalpp::canvas canvas({1, 1});
    canvas[0][0] = '?';
    
    container_.set_size({1, 1});
    
    terminalpp::canvas_view canvas_view(canvas);
    munin::context context(canvas_view);
    container_.draw(context, {{0, 0}, {1, 1}});
    
    ASSERT_EQ(terminalpp::element('?'), canvas[0][0]);
}

TEST_F(a_container, reports_a_preferred_size_change_when_a_component_is_removed)
{
    auto component = std::make_shared<StrictMock<mock_component>>();
    
    container_.add_component(component);
    container_.remove_component(component);

    ASSERT_EQ(2, preferred_size_changed_count);
}

