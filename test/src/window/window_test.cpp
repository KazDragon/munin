#include "window_test.hpp"
#include <gtest/gtest.h>

using testing::Invoke;
using testing::Return;
using testing::_;

TEST_F(a_window, sets_the_size_of_the_content_when_having_its_size_set)
{
    auto const size = terminalpp::extent({80, 24});
    EXPECT_CALL(*content_, do_set_size(size));
    
    window_->set_size(size);
}


TEST_F(a_window, passes_events_to_the_content)
{
    struct tag {};

    boost::any result;
    
    EXPECT_CALL(*content_, do_event(_))
        .WillOnce(Invoke([&result](boost::any const &event){ result = event; }));
    
    window_->event(tag{});

    auto *ptag = boost::any_cast<tag>(&result);
    ASSERT_NE(nullptr, ptag);
}
