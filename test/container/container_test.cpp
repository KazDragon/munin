#include <munin/container.hpp>
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include "../mock/component.hpp"
#include "../mock/layout.hpp"
#include <gtest/gtest.h>

using testing::StrictMock;
using testing::_;

TEST(make_container, makes_a_new_container)
{
    std::shared_ptr<munin::container> container = munin::make_container();
    ASSERT_FALSE(container == nullptr);
}

class a_new_container : public testing::Test
{
protected :
    void SetUp() override
    {
        container_.on_focus_set.connect(
            [this]()
            {
                ++focus_set_count;
            });
            
        container_.on_focus_lost.connect(
            [this]()
            {
                ++focus_lost_count;
            });
    }
    
    munin::container container_;
    
    int focus_set_count = 0;
    int focus_lost_count = 0;
};

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

TEST_F(a_new_container, is_disabled)
{
    ASSERT_FALSE(container_.is_enabled());
}

TEST_F(a_new_container, cannot_be_enabled)
{
    container_.enable();
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

TEST(a_container_with_no_elements, does_not_lay_the_container_out_when_a_component_is_added)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);

    munin::container container;
    container.set_layout(std::move(layout));
}

TEST(a_container_with_elements, lays_the_container_out_when_a_component_is_added)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);
    auto component = std::make_shared<StrictMock<mock_component>>();

    munin::container container;
    container.add_component(component);
    
    EXPECT_CALL(*layout, do_layout(_, _, _));
    container.set_layout(std::move(layout));
}

class a_container : public testing::Test
{
protected :
    void SetUp() override
    {
        container_.on_preferred_size_changed.connect(
            [this]()
            {
                ++preferred_size_changed_count; 
            });
    }
    
    munin::container container_;
    int preferred_size_changed_count = 0;
};

TEST_F(a_container, lays_out_the_container_when_a_component_is_added)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);
    auto component = std::make_shared<StrictMock<mock_component>>();
    
    EXPECT_CALL(*layout, do_layout(_, _, _));
    container_.set_layout(std::move(layout));
    container_.add_component(component);
}

TEST_F(a_container, reports_a_preferred_size_change_when_a_component_is_added)
{
    auto component = std::make_shared<StrictMock<mock_component>>();
    
    container_.add_component(component);
    ASSERT_EQ(1, preferred_size_changed_count);
}

TEST_F(a_container, lays_out_the_container_when_a_component_is_removed)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);
    auto component = std::make_shared<StrictMock<mock_component>>();

    container_.add_component(component);

    EXPECT_CALL(*layout, do_layout(_, _, _))
        .Times(2);

    container_.set_layout(std::move(layout));

    container_.remove_component(component);
}

TEST_F(a_container, reports_a_preferred_size_change_when_a_component_is_removed)
{
    auto component = std::make_shared<StrictMock<mock_component>>();
    
    container_.add_component(component);
    container_.remove_component(component);

    ASSERT_EQ(2, preferred_size_changed_count);
}
