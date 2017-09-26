#include <munin/window.hpp>
#include <terminalpp/ansi_terminal.hpp>
#include "mock/component.hpp"
#include "mock/terminal.hpp"
#include <gtest/gtest.h>

using testing::Invoke;
using testing::Return;
using testing::_;

class a_window : public testing::Test
{
protected:
    a_window()
      : window_(new munin::window(terminal_, content_))
    {
    }
    
    std::shared_ptr<mock_terminal>  terminal_ = std::make_shared<mock_terminal>();
    std::shared_ptr<mock_component> content_ = std::make_shared<mock_component>();
    std::unique_ptr<munin::window>  window_;
};

class a_new_window : public a_window
{
};

TEST_F(a_new_window, sets_the_size_of_the_content_when_having_its_size_set)
{
    auto const size = terminalpp::extent({80, 24});
    EXPECT_CALL(*content_, do_set_size(size));
    
    window_->set_size(size);
}


TEST_F(a_window, reads_single_data_from_the_terminal_and_passes_the_result_to_the_content)
{
    terminalpp::virtual_key vk_test{ terminalpp::vk::uppercase_t };
    
    EXPECT_CALL(*terminal_, read("test"))
        .WillOnce(Return(
            std::vector<terminalpp::token>{vk_test}));
    
    boost::any result;
    
    EXPECT_CALL(*content_, do_event(_))
        .WillOnce(Invoke([&result](boost::any const &event){ result = event; }));
    
    window_->data("test");

    auto *pvk = boost::any_cast<terminalpp::virtual_key>(&result);
    ASSERT_NE(nullptr, pvk);
    ASSERT_EQ(vk_test, *pvk);
}

TEST_F(a_window, reads_many_data_from_the_terminal_and_passes_the_result_to_the_content)
{
    terminalpp::virtual_key vk_test0{ terminalpp::vk::lowercase_t };
    terminalpp::virtual_key vk_test1{ terminalpp::vk::question_mark };
    
    EXPECT_CALL(*terminal_, read("test"))
        .WillOnce(Return(
            std::vector<terminalpp::token>{vk_test0, vk_test1}));
    
    std::vector<boost::any> result;
    
    EXPECT_CALL(*content_, do_event(_))
        .WillRepeatedly(Invoke(
            [&result](boost::any const &event)
            {
                result.push_back(event); 
            }));
    
    window_->data("test");

    ASSERT_EQ(2u, result.size());
    
    auto *pvk0 = boost::any_cast<terminalpp::virtual_key>(&result[0]);
    ASSERT_NE(nullptr, pvk0);
    ASSERT_EQ(vk_test0, *pvk0);

    auto *pvk1 = boost::any_cast<terminalpp::virtual_key>(&result[1]);
    ASSERT_NE(nullptr, pvk1);
    ASSERT_EQ(vk_test1, *pvk1);
}
