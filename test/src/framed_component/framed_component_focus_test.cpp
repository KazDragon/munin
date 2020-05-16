#include "mock/component.hpp"
#include "mock/frame.hpp"
#include <munin/framed_component.hpp>
#include <gtest/gtest.h>

using testing::Invoke;
using testing::Return;
using testing::_;

namespace {

class a_framed_component_with_an_inner_component
  : public testing::Test
{
protected:
    void SetUp() override
    {
        framed_component_->on_focus_set.connect([this]{++focus_set_count_;});
        framed_component_->on_focus_lost.connect([this]{++focus_lost_count_;});

        // This is technically a test, but it cuts across all tests: the frame
        // is ignored when reasoning about focus for framed components
        EXPECT_CALL(*mock_frame_, do_set_focus()).Times(0);
        EXPECT_CALL(*mock_frame_, do_lose_focus()).Times(0);
        EXPECT_CALL(*mock_frame_, do_focus_next()).Times(0);
        EXPECT_CALL(*mock_frame_, do_focus_previous()).Times(0);

        // It's possible that during focus transitions of the inner component,
        // the frame is asked whether it has focus.  For our purposes, the
        // answer is always no.
        ON_CALL(*mock_frame_, do_has_focus())
            .WillByDefault(Return(false));
    }

    std::shared_ptr<mock_frame> mock_frame_{make_mock_frame()};
    std::shared_ptr<mock_component> mock_inner_{make_mock_component()};
    std::shared_ptr<munin::framed_component> framed_component_{
        munin::make_framed_component(mock_frame_, mock_inner_)
    };

    int focus_set_count_{0};
    int focus_lost_count_{0};
};

class a_framed_component_with_an_inner_component_that_takes_no_focus 
  : public a_framed_component_with_an_inner_component
{
protected:
    void SetUp() override
    {
        a_framed_component_with_an_inner_component::SetUp();

        ON_CALL(*mock_inner_, do_set_focus())
            .WillByDefault(Return());
        ON_CALL(*mock_inner_, do_lose_focus())
            .WillByDefault(Return());
        ON_CALL(*mock_inner_, do_focus_next())
            .WillByDefault(Return());
        ON_CALL(*mock_inner_, do_focus_previous())
            .WillByDefault(Return());
        ON_CALL(*mock_inner_, do_has_focus())
            .WillByDefault(Return(false));
    }
};

}

TEST_F(a_framed_component_with_an_inner_component_that_takes_no_focus, refuses_set_focus)
{
    framed_component_->set_focus();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(a_framed_component_with_an_inner_component_that_takes_no_focus, refuses_next_focus)
{
    framed_component_->focus_next();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(a_framed_component_with_an_inner_component_that_takes_no_focus, refuses_previous_focus)
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
    void SetUp() override
    {
        a_framed_component_with_an_inner_component::SetUp();

        ON_CALL(*mock_inner_, do_set_focus())
            .WillByDefault(Invoke([this]{
                has_focus_ = true;
                mock_inner_->on_focus_set();
            }));

        ON_CALL(*mock_inner_, do_focus_next())
            .WillByDefault(Invoke([this]{
                has_focus_ = true;
                mock_inner_->on_focus_set();
            }));

        ON_CALL(*mock_inner_, do_focus_previous())
            .WillByDefault(Invoke([this]{
                has_focus_ = true;
                mock_inner_->on_focus_set();
            }));

        ON_CALL(*mock_inner_, do_has_focus())
            .WillByDefault(Invoke([this]{return has_focus_;}));
    }

    bool has_focus_{false};
};

}

TEST_F(a_framed_component_with_an_inner_component_that_can_have_focus_set, sets_inner_component_focus_when_setting_focus)
{
    framed_component_->set_focus();

    ASSERT_EQ(true, framed_component_->has_focus());
    ASSERT_EQ(1, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(a_framed_component_with_an_inner_component_that_can_have_focus_set, sets_inner_component_next_focus_when_setting_next_focus)
{
    framed_component_->focus_next();

    ASSERT_EQ(true, framed_component_->has_focus());
    ASSERT_EQ(1, focus_set_count_);
    ASSERT_EQ(0, focus_lost_count_);
}

TEST_F(a_framed_component_with_an_inner_component_that_can_have_focus_set, sets_inner_component_previous_focus_when_setting_previous_focus)
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
    void SetUp() override
    {
        a_framed_component_with_an_inner_component::SetUp();

        ON_CALL(*mock_inner_, do_lose_focus())
            .WillByDefault(Invoke([this]{
                has_focus_ = false;
                mock_inner_->on_focus_lost();
            }));

        ON_CALL(*mock_inner_, do_has_focus())
            .WillByDefault(Invoke([this]{return has_focus_;}));
    }

    bool has_focus_{false};
};

}

TEST_F(a_framed_component_with_an_inner_component_that_has_focus, loses_innert_component_focus_when_losing_focus)
{
    framed_component_->lose_focus();

    ASSERT_EQ(false, framed_component_->has_focus());
    ASSERT_EQ(0, focus_set_count_);
    ASSERT_EQ(1, focus_lost_count_);
}