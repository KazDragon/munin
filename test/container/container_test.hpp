#pragma once
#include "../mock/component.hpp"
#include <munin/container.hpp>
#include <gtest/gtest.h>

class container_test_base : public testing::Test
{
protected :
    void SetUp() override
    {
        Test::SetUp();

        container.on_preferred_size_changed.connect(
            [this]()
            {
                ++preferred_size_changed_count;
            });

        container.on_focus_set.connect(
            [this]()
            {
                ++focus_set_count;
            });

        container.on_focus_lost.connect(
            [this]()
            {
                ++focus_lost_count;
            });
    }

    munin::container container;

    int preferred_size_changed_count = 0;
    int focus_set_count = 0;
    int focus_lost_count = 0;
};

class a_container : public container_test_base
{
};

class a_new_container : public container_test_base
{
};

class a_container_with_one_component : public a_container
{
protected :
    void SetUp() override
    {
        a_container::SetUp();

        container.add_component(component);
        preferred_size_changed_count = 0;
    }

    std::shared_ptr<mock_component> component =
        std::make_shared<mock_component>();
};

class a_container_with_one_component_that_has_focus : public a_container_with_one_component
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_one_component::SetUp();

        EXPECT_CALL(*component, do_set_focus());
        EXPECT_CALL(*component, do_has_focus())
            .WillOnce(Return(true));

        container.set_focus();

        assert(container.has_focus());

        focus_set_count = 0;
        preferred_size_changed_count = 0;
    }
};

class a_container_with_two_components : public a_container
{
protected :
    void SetUp() override
    {
        a_container::SetUp();

        container.add_component(component0);
        container.add_component(component1);
    }

    std::shared_ptr<mock_component> component0 =
        std::make_shared<mock_component>();
    std::shared_ptr<mock_component> component1=
        std::make_shared<mock_component>();
};

class a_container_with_two_components_where_the_first_has_focus : public a_container_with_two_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        EXPECT_CALL(*component0, do_focus_next());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));

        container.focus_next();

        assert(container.has_focus());

        focus_set_count = 0;
        preferred_size_changed_count = 0;
    }
};

class a_container_with_two_components_where_the_last_has_focus : public a_container_with_two_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        EXPECT_CALL(*component1, do_focus_previous());
        EXPECT_CALL(*component1, do_has_focus())
            .WillOnce(Return(true));

        container.focus_previous();

        assert(container.has_focus());
        focus_set_count = 0;
    }
};

class a_container_with_three_components : public a_container_with_two_components
{
protected :
    void SetUp() override
    {
        a_container_with_two_components::SetUp();

        container.add_component(component2);
    }

    std::shared_ptr<mock_component> component2 =
        std::make_shared<mock_component>();
};

class a_container_with_three_components_where_the_first_has_focus : public a_container_with_three_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_three_components::SetUp();

        EXPECT_CALL(*component0, do_focus_next());
        EXPECT_CALL(*component0, do_has_focus())
            .WillOnce(Return(true));

        container.focus_next();

        assert(container.has_focus());

        focus_set_count = 0;
        preferred_size_changed_count = 0;
    }
};

class a_container_with_three_components_where_the_last_has_focus : public a_container_with_three_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_three_components::SetUp();

        EXPECT_CALL(*component2, do_focus_previous());
        EXPECT_CALL(*component2, do_has_focus())
            .WillOnce(Return(true));

        container.focus_previous();

        assert(container.has_focus());

        focus_set_count = 0;
        preferred_size_changed_count = 0;
    }
};
