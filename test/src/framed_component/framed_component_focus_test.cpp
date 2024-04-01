#include "mock/component.hpp"
#include "mock/frame.hpp"

#include <gtest/gtest.h>
#include <munin/framed_component.hpp>

using testing::Assign;
using testing::DoAll;
using testing::Return;
using testing::ReturnPointee;

namespace {

class a_framed_component_with_an_inner_component : public testing::Test
{
protected:
    a_framed_component_with_an_inner_component()
    {
        framed_component_->on_focus_set.connect([this] { ++focus_set_count_; });
        framed_component_->on_focus_lost.connect(
            [this] { ++focus_lost_count_; });

        // It's possible that during focus transitions of the inner component,
        // the frame is asked whether it has focus.  For our purposes, the
        // answer is always no.
        ON_CALL(*mock_frame_, do_has_focus()).WillByDefault(Return(false));
    }

    std::shared_ptr<mock_frame> mock_frame_{make_mock_frame()};
    std::shared_ptr<mock_component> mock_inner_{make_mock_component()};
    std::shared_ptr<munin::framed_component> framed_component_{
        munin::make_framed_component(mock_frame_, mock_inner_)};

    int focus_set_count_{0};
    int focus_lost_count_{0};
};

class a_framed_component_with_an_inner_component_that_takes_no_focus
  : public a_framed_component_with_an_inner_component
{
protected:
    a_framed_component_with_an_inner_component_that_takes_no_focus()
    {
        ON_CALL(*mock_inner_, do_has_focus()).WillByDefault(Return(false));
    }
};

}  // namespace

TEST_F(
    a_framed_component_with_an_inner_component_that_takes_no_focus,
    refuses_set_focus)
{
    framed_component_->set_focus();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(
    a_framed_component_with_an_inner_component_that_takes_no_focus,
    refuses_next_focus)
{
    framed_component_->focus_next();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(
    a_framed_component_with_an_inner_component_that_takes_no_focus,
    refuses_previous_focus)
{
    framed_component_->focus_previous();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

namespace {

class a_framed_component_with_an_inner_component_that_can_have_focus_set
  : public a_framed_component_with_an_inner_component
{
protected:
    a_framed_component_with_an_inner_component_that_can_have_focus_set()
    {
        ON_CALL(*mock_inner_, do_set_focus())
            .WillByDefault(DoAll(
                Assign(&has_focus_, true),
                std::ref(mock_inner_->on_focus_set)));

        ON_CALL(*mock_inner_, do_focus_next())
            .WillByDefault(DoAll(
                Assign(&has_focus_, true),
                std::ref(mock_inner_->on_focus_set)));

        ON_CALL(*mock_inner_, do_focus_previous())
            .WillByDefault(DoAll(
                Assign(&has_focus_, true),
                std::ref(mock_inner_->on_focus_set)));

        ON_CALL(*mock_inner_, do_has_focus())
            .WillByDefault(ReturnPointee(&has_focus_));
    }

    bool has_focus_{false};
};

}  // namespace

TEST_F(
    a_framed_component_with_an_inner_component_that_can_have_focus_set,
    sets_inner_component_focus_when_setting_focus)
{
    framed_component_->set_focus();

    ASSERT_EQ(true, framed_component_->has_focus());
    ASSERT_EQ(1, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(
    a_framed_component_with_an_inner_component_that_can_have_focus_set,
    sets_inner_component_next_focus_when_setting_next_focus)
{
    framed_component_->focus_next();

    ASSERT_EQ(true, framed_component_->has_focus());
    ASSERT_EQ(1, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(
    a_framed_component_with_an_inner_component_that_can_have_focus_set,
    sets_inner_component_previous_focus_when_setting_previous_focus)
{
    framed_component_->focus_previous();

    ASSERT_EQ(true, framed_component_->has_focus());
    ASSERT_EQ(1, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

namespace {

class a_framed_component_with_an_inner_component_that_has_focus
  : public a_framed_component_with_an_inner_component
{
protected:
    a_framed_component_with_an_inner_component_that_has_focus()
    {
        ON_CALL(*mock_inner_, do_lose_focus())
            .WillByDefault(DoAll(
                Assign(&has_focus_, false),
                std::ref(mock_inner_->on_focus_lost)));

        ON_CALL(*mock_inner_, do_has_focus())
            .WillByDefault(ReturnPointee(&has_focus_));
    }

    bool has_focus_{true};
};

}  // namespace

TEST_F(
    a_framed_component_with_an_inner_component_that_has_focus,
    loses_inner_component_focus_when_losing_focus)
{
    framed_component_->lose_focus();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(1, focus_lost_count_);
}