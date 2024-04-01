#pragma once
#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/container.hpp>

class container_test_base : public testing::Test
{
protected:
    container_test_base()
    {
        container_.on_preferred_size_changed.connect(
            [this]() { ++preferred_size_changed_count_; });

        container_.on_focus_set.connect([this]() { ++focus_set_count_; });

        container_.on_focus_lost.connect([this]() { ++focus_lost_count_; });

        container_.on_cursor_position_changed.connect(
            [this]() { ++cursor_position_changed_count_; });

        container_.on_cursor_state_changed.connect(
            [this]() { ++cursor_state_changed_count_; });

        container_.on_redraw.connect([this](auto const &) { ++redraw_count_; });
    }

    void reset_counters()
    {
        preferred_size_changed_count_ = 0;
        focus_set_count_ = 0;
        focus_lost_count_ = 0;
        cursor_position_changed_count_ = 0;
        cursor_state_changed_count_ = 0;
        redraw_count_ = 0;
    }

    munin::container container_;

    int preferred_size_changed_count_ = 0;
    int focus_set_count_ = 0;
    int focus_lost_count_ = 0;
    int cursor_position_changed_count_ = 0;
    int cursor_state_changed_count_ = 0;
    int redraw_count_ = 0;
};

class a_container : public container_test_base
{
};

class a_new_container : public container_test_base
{
};

class a_container_with_one_component : public a_container
{
protected:
    a_container_with_one_component()
    {
        container_.add_component(component_);

        reset_counters();
    }

    std::shared_ptr<mock_component> component_{make_mock_component()};
};

class a_container_with_one_component_that_has_focus
  : public a_container_with_one_component
{
protected:
    a_container_with_one_component_that_has_focus()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*component_, do_set_focus());
            EXPECT_CALL(*component_, do_has_focus()).WillOnce(Return(true));
        }

        container_.set_focus();

        assert(container_.has_focus());
        reset_counters();
    }
};

class a_container_with_two_components : public a_container
{
protected:
    a_container_with_two_components()
    {
        container_.add_component(component0_);
        container_.add_component(component1_);
    }

    std::shared_ptr<mock_component> component0_{make_mock_component()};
    std::shared_ptr<mock_component> component1_{make_mock_component()};
};

class a_container_with_two_components_where_the_first_has_focus
  : public a_container_with_two_components
{
protected:
    a_container_with_two_components_where_the_first_has_focus()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*component0_, do_has_focus())
                .WillRepeatedly(Return(false));
            EXPECT_CALL(*component0_, do_focus_next());
            EXPECT_CALL(*component0_, do_has_focus())
                .WillRepeatedly(Return(true));
        }

        container_.focus_next();

        assert(container_.has_focus());
        reset_counters();
    }
};

class a_container_with_two_components_where_the_last_has_focus
  : public a_container_with_two_components
{
protected:
    a_container_with_two_components_where_the_last_has_focus()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*component1_, do_has_focus())
                .WillRepeatedly(Return(false));
            EXPECT_CALL(*component1_, do_focus_previous());
            EXPECT_CALL(*component1_, do_has_focus())
                .WillRepeatedly(Return(true));
        }

        container_.focus_previous();

        assert(container_.has_focus());
        reset_counters();
    }
};

class a_container_with_three_components : public a_container_with_two_components
{
protected:
    a_container_with_three_components()
    {
        container_.add_component(component2_);
    }

    std::shared_ptr<mock_component> component2_{make_mock_component()};
};

class a_container_with_three_components_where_the_first_has_focus
  : public a_container_with_three_components
{
protected:
    a_container_with_three_components_where_the_first_has_focus()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*component0_, do_has_focus())
                .WillRepeatedly(Return(false));
            EXPECT_CALL(*component0_, do_focus_next());
            EXPECT_CALL(*component0_, do_has_focus())
                .WillRepeatedly(Return(true));
        }

        container_.focus_next();

        assert(container_.has_focus());
        reset_counters();
    }
};

class a_container_with_three_components_where_the_last_has_focus
  : public a_container_with_three_components
{
protected:
    a_container_with_three_components_where_the_last_has_focus()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*component2_, do_has_focus())
                .WillRepeatedly(Return(false));
            EXPECT_CALL(*component2_, do_focus_previous());
            EXPECT_CALL(*component2_, do_has_focus())
                .WillRepeatedly(Return(true));
        }

        container_.focus_previous();

        assert(container_.has_focus());
        reset_counters();
    }
};

template <class TestData>
class containers_with_a_component : public testing::TestWithParam<TestData>
{
protected:
    containers_with_a_component()
    {
        container_.add_component(component_);
    }

    munin::container container_;
    std::shared_ptr<mock_component> component_{make_mock_component()};
};

template <class TestData>
class containers_with_a_focussed_component
  : public containers_with_a_component<TestData>
{
protected:
    containers_with_a_focussed_component()
    {
        using testing::InSequence;
        using testing::Return;

        {
            InSequence s1;
            EXPECT_CALL(*this->component_, do_set_focus());
            EXPECT_CALL(*this->component_, do_has_focus())
                .WillOnce(Return(true));
        }

        this->container_.set_focus();

        assert(this->container_.has_focus());
    }
};
