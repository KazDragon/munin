#include <munin/container.hpp>
#include <munin/context.hpp>
#include "container_test.hpp"
#include "../mock/component.hpp"
#include "../mock/layout.hpp"
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;
using testing::Return;
using testing::StrictMock;

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

TEST_F(a_container, lays_out_the_container_when_its_size_is_changed)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);

    EXPECT_CALL(*layout, do_layout(_, _, _));

    container_.set_layout(std::move(layout));

    container_.set_size({1, 1});
}

TEST_F(a_container, has_the_preferred_size_of_its_layout)
{
    auto layout = std::unique_ptr<StrictMock<mock_layout>>(new StrictMock<mock_layout>);
    auto const expected_result = terminalpp::extent{42, 69};

    EXPECT_CALL(*layout, do_get_preferred_size(_, _))
        .WillOnce(Return(expected_result));

    container_.set_layout(std::move(layout));

    ASSERT_EQ(expected_result, container_.get_preferred_size());
}
