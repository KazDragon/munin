#include "container_test.hpp"
#include "../mock/layout.hpp"

using testing::_;
using testing::Return;

TEST(a_container_with_no_elements, does_not_lay_the_container_out_when_a_component_is_added)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);

    munin::container container;
    container.set_layout(std::move(layout));
}

TEST(a_container_with_elements, lays_the_container_out_when_a_component_is_added)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);
    auto component = std::make_shared<mock_component>();

    munin::container container;
    container.add_component(component);

    EXPECT_CALL(*layout, do_layout(_, _, _));
    container.set_layout(std::move(layout));
}

TEST_F(a_container, lays_out_the_container_when_a_component_is_added)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);
    auto component = std::make_shared<mock_component>();

    EXPECT_CALL(*layout, do_layout(_, _, _))
        .Times(2);
    container.set_layout(std::move(layout));
    container.add_component(component);
}

TEST_F(a_container, reports_a_preferred_size_change_when_a_component_is_added)
{
    auto component = std::make_shared<mock_component>();

    container.add_component(component);
    ASSERT_EQ(1, preferred_size_changed_count);
}

TEST_F(a_container, lays_out_the_container_when_a_component_is_removed)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);
    auto component = std::make_shared<mock_component>();

    container.add_component(component);

    EXPECT_CALL(*layout, do_layout(_, _, _))
        .Times(2);

    container.set_layout(std::move(layout));

    container.remove_component(component);
}

TEST_F(a_container, lays_out_the_container_when_its_size_is_changed)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);

    EXPECT_CALL(*layout, do_layout(_, _, _))
        .Times(2);

    container.set_layout(std::move(layout));
    container.set_size({1, 1});
}

TEST_F(a_container, has_the_preferred_size_of_its_layout)
{
    auto layout = std::unique_ptr<mock_layout>(new mock_layout);
    auto const expected_result = terminalpp::extent{42, 69};

    EXPECT_CALL(*layout, do_get_preferred_size(_, _))
        .WillOnce(Return(expected_result));

    container.set_layout(std::move(layout));

    ASSERT_EQ(expected_result, container.get_preferred_size());
}
