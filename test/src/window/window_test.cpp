#include "window_test.hpp"
#include <gtest/gtest.h>

using testing::_;
using testing::SaveArg;

TEST_F(a_window, passes_events_to_the_content)
{
  struct tag
  {
  };

  boost::any result;

  EXPECT_CALL(*content_, do_event(_)).WillOnce(SaveArg<0>(&result));

  window_->event(tag{});

  auto *ptag = boost::any_cast<tag>(&result);
  ASSERT_NE(nullptr, ptag);
}
