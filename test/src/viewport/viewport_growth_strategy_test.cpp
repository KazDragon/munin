#include "viewport_test.hpp"
#include <gtest/gtest.h>

using testing::Return;

namespace {

class a_viewport_with_a_vertical_growth_strategy
  : public a_viewport
{
public:
    a_viewport_with_a_vertical_growth_strategy()
    {
        viewport_ = munin::make_viewport(
            tracked_component_,
            munin::make_vertical_viewport_growth_strategy());
    }
};

}

TEST_F(a_viewport_with_a_vertical_growth_strategy, is_granted_vertical_but_not_horizontal_size_when_the_viewport_is_resized)
{
    auto const tracked_preferred_size = terminalpp::extent{10, 10};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_preferred_size));

    viewport_->set_size({5, 5});

    auto const expected_tracked_size = terminalpp::extent{5, 10};
    ASSERT_EQ(expected_tracked_size, tracked_component_->get_size());
}

namespace {

class a_viewport_with_a_horizontal_growth_strategy
  : public a_viewport
{
public:
    a_viewport_with_a_horizontal_growth_strategy()
    {
        viewport_ = munin::make_viewport(
            tracked_component_,
            munin::make_horizontal_viewport_growth_strategy());
    }
};

}

TEST_F(a_viewport_with_a_horizontal_growth_strategy, is_granted_horizontal_but_not_vertical_size_when_the_viewport_is_resized)
{
    auto const tracked_preferred_size = terminalpp::extent{10, 10};

    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(tracked_preferred_size));

    viewport_->set_size({5, 5});

    auto const expected_tracked_size = terminalpp::extent{10, 5};
    ASSERT_EQ(expected_tracked_size, tracked_component_->get_size());
}

