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

        container_.on_preferred_size_changed.connect(
            [this]()
            {
                ++preferred_size_changed_count;
            });

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
public :
    a_container_with_one_component()
      : component_(std::make_shared<mock_component>())
    {
    }

protected :
    void SetUp() override
    {
        a_container::SetUp();

        container_.add_component(component_);
    }

    std::shared_ptr<mock_component> component_;
};

class a_container_with_two_components : public a_container
{
public :
    a_container_with_two_components()
      : component0_(std::make_shared<mock_component>()),
        component1_(std::make_shared<mock_component>())
    {
    }

protected :
    void SetUp() override
    {
        a_container::SetUp();

        container_.add_component(component0_);
        container_.add_component(component1_);
    }

    std::shared_ptr<mock_component> component0_;
    std::shared_ptr<mock_component> component1_;
};

class a_container_with_two_components_where_the_first_has_focus : public a_container_with_two_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        EXPECT_CALL(*component0_, do_focus_next());
        EXPECT_CALL(*component0_, do_has_focus())
            .WillOnce(Return(true));

        container_.focus_next();

        assert(container_.has_focus());
        focus_set_count = 0;
    }
};

class a_container_with_two_components_where_the_last_has_focus : public a_container_with_two_components
{
protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        EXPECT_CALL(*component1_, do_focus_previous());
        EXPECT_CALL(*component1_, do_has_focus())
            .WillOnce(Return(true));

        container_.focus_previous();

        assert(container_.has_focus());
        focus_set_count = 0;
    }
};

class a_container_with_three_components_where_the_first_has_focus : public a_container_with_two_components
{
public :
    a_container_with_three_components_where_the_first_has_focus()
      : component2_(std::make_shared<mock_component>())
    {
    }

protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        container_.add_component(component2_);

        EXPECT_CALL(*component0_, do_focus_next());
        EXPECT_CALL(*component0_, do_has_focus())
            .WillOnce(Return(true));

        container_.focus_next();

        assert(container_.has_focus());
        focus_set_count = 0;
    }

    std::shared_ptr<mock_component> component2_;
};

class a_container_with_three_components_where_the_last_has_focus : public a_container_with_two_components
{
public :
    a_container_with_three_components_where_the_last_has_focus()
      : component2_(std::make_shared<mock_component>())
    {
    }

protected :
    void SetUp() override
    {
        using testing::Return;

        a_container_with_two_components::SetUp();

        container_.add_component(component2_);

        EXPECT_CALL(*component2_, do_focus_previous());
        EXPECT_CALL(*component2_, do_has_focus())
            .WillOnce(Return(true));

        container_.focus_previous();

        assert(container_.has_focus());
        focus_set_count = 0;
    }

    std::shared_ptr<mock_component> component2_;
};
