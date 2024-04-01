#include "mock/animator.hpp"

#include "mock/component.hpp"

#include <gtest/gtest.h>

#include <chrono>

using namespace std::literals;
using testing::Return;

namespace {

class an_animator : public testing::Test
{
protected:
    an_animator() : now_(std::chrono::steady_clock::now())
    {
        ON_CALL(*animator_, do_now()).WillByDefault(Return(now_));

        component_->on_redraw.connect(
            [this](auto) { this->redraw_requested_ = true; });
    }

    std::shared_ptr<mock_animator> animator_{make_mock_animator()};
    std::shared_ptr<mock_component> component_{make_mock_component()};

    std::chrono::steady_clock::time_point now_;

    bool redraw_requested_{};
};

class an_animator_with_no_ongoing_animations : public an_animator
{
};

}  // namespace

TEST_F(
    an_animator_with_no_ongoing_animations,
    resets_the_timer_for_the_first_animation_request)
{
    auto const expiry_time = now_ + 5s;

    EXPECT_CALL(*animator_, reset_timer(expiry_time));

    animator_->redraw_component_at(component_, {{}, {}}, expiry_time);

    ASSERT_FALSE(redraw_requested_);
}

namespace {

class an_animator_with_an_animation_that_expires_in_5s : public an_animator
{
protected:
    an_animator_with_an_animation_that_expires_in_5s() : expiry_time_(now_ + 5s)
    {
        animator_->redraw_component_at(component_, {{}, {}}, expiry_time_);
    }

    std::chrono::steady_clock::time_point expiry_time_;
};

}  // namespace

TEST_F(
    an_animator_with_an_animation_that_expires_in_5s,
    does_not_request_a_redraw_when_redrawing_components_before_5s_has_expired)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 4900ms));

    animator_->redraw_components();

    ASSERT_FALSE(redraw_requested_);
}

TEST_F(
    an_animator_with_an_animation_that_expires_in_5s,
    requests_a_redraw_when_redrawing_components_when_5s_has_expired)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 5s));

    animator_->redraw_components();

    ASSERT_TRUE(redraw_requested_);
}

TEST_F(
    an_animator_with_an_animation_that_expires_in_5s,
    requests_a_redraw_when_redrawing_components_after_5s_has_expired)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 6s));

    animator_->redraw_components();

    ASSERT_TRUE(redraw_requested_);
}

namespace {

class an_animator_with_animations_that_expires_in_5s_and_10s
  : public testing::Test
{
protected:
    an_animator_with_animations_that_expires_in_5s_and_10s()
      : now_(std::chrono::steady_clock::now()),
        expiry_time0_(now_ + 5s),
        expiry_time1_(now_ + 10s),
        redraw0_requested_(false),
        redraw1_requested_(false)
    {
        ON_CALL(*animator_, do_now()).WillByDefault(Return(now_));

        component0_->on_redraw.connect(
            [this](auto) { this->redraw0_requested_ = true; });

        bool redraw1_requested = false;
        component1_->on_redraw.connect(
            [this](auto) { this->redraw1_requested_ = true; });

        animator_->redraw_component_at(component0_, {{}, {}}, expiry_time0_);
        animator_->redraw_component_at(component1_, {{}, {}}, expiry_time1_);
    }

    std::shared_ptr<mock_animator> animator_{make_mock_animator()};
    std::shared_ptr<mock_component> component0_{make_mock_component()};
    std::shared_ptr<mock_component> component1_{make_mock_component()};

    std::chrono::steady_clock::time_point now_;
    std::chrono::steady_clock::time_point expiry_time0_;
    std::chrono::steady_clock::time_point expiry_time1_;

    bool redraw0_requested_;
    bool redraw1_requested_;
};

}  // namespace

TEST_F(
    an_animator_with_animations_that_expires_in_5s_and_10s,
    does_not_request_redraws_when_redrawing_components_before_5s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 4900ms));

    animator_->redraw_components();

    ASSERT_FALSE(redraw0_requested_);
    ASSERT_FALSE(redraw1_requested_);
}

TEST_F(
    an_animator_with_animations_that_expires_in_5s_and_10s,
    requests_a_redraw_of_component0_when_redrawing_components_when_5s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 5s));

    animator_->redraw_components();

    ASSERT_TRUE(redraw0_requested_);
    ASSERT_FALSE(redraw1_requested_);
}

TEST_F(
    an_animator_with_animations_that_expires_in_5s_and_10s,
    requests_a_redraw_of_component0_when_redrawing_components_when_more_than_5s_but_less_than_10s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 9900ms));

    animator_->redraw_components();

    ASSERT_TRUE(redraw0_requested_);
    ASSERT_FALSE(redraw1_requested_);
}

TEST_F(
    an_animator_with_animations_that_expires_in_5s_and_10s,
    requests_a_redraw_of_component0_and_component1_when_redrawing_components_when_10s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 10s));

    animator_->redraw_components();

    ASSERT_TRUE(redraw0_requested_);
    ASSERT_TRUE(redraw1_requested_);
}

TEST_F(
    an_animator_with_animations_that_expires_in_5s_and_10s,
    requests_a_redraw_of_component0_and_component1_when_redrawing_components_when_5s_then_10s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 5s));

    animator_->redraw_components();

    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 10s));

    ASSERT_TRUE(redraw0_requested_);
    ASSERT_FALSE(redraw1_requested_);

    redraw0_requested_ = false;

    animator_->redraw_components();

    ASSERT_FALSE(redraw0_requested_);
    ASSERT_TRUE(redraw1_requested_);
}

namespace {

class an_animator_with_animations_that_expires_in_5s : public testing::Test
{
protected:
    an_animator_with_animations_that_expires_in_5s()
      : now_(std::chrono::steady_clock::now()),
        expiry_time0_(now_ + 5s),
        expiry_time1_(now_ + 5s),
        redraw0_requested_(false),
        redraw1_requested_(false)
    {
        ON_CALL(*animator_, do_now()).WillByDefault(Return(now_));

        component0_->on_redraw.connect(
            [this](auto) { this->redraw0_requested_ = true; });

        bool redraw1_requested = false;
        component1_->on_redraw.connect(
            [this](auto) { this->redraw1_requested_ = true; });

        animator_->redraw_component_at(component0_, {{}, {}}, expiry_time0_);
        animator_->redraw_component_at(component1_, {{}, {}}, expiry_time1_);
    }

    std::shared_ptr<mock_animator> animator_{make_mock_animator()};
    std::shared_ptr<mock_component> component0_{make_mock_component()};
    std::shared_ptr<mock_component> component1_{make_mock_component()};

    std::chrono::steady_clock::time_point now_;
    std::chrono::steady_clock::time_point expiry_time0_;
    std::chrono::steady_clock::time_point expiry_time1_;

    bool redraw0_requested_;
    bool redraw1_requested_;
};

}  // namespace

TEST_F(
    an_animator_with_animations_that_expires_in_5s,
    requests_a_redraw_of_component0_and_component1_when_redrawing_components_when_5s_has_passed)
{
    ON_CALL(*animator_, do_now()).WillByDefault(Return(now_ + 5s));

    animator_->redraw_components();

    ASSERT_TRUE(redraw0_requested_);
    ASSERT_TRUE(redraw1_requested_);
}
