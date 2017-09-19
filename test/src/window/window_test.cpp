#include <munin/window.hpp>
#include <terminalpp/ansi_terminal.hpp>
#include "mock/component.hpp"
#include <gtest/gtest.h>

class a_new_window : public testing::Test
{
public :
    a_new_window()
    {
    }
    
protected :
    void SetUp() override
    {
        content_ = std::make_shared<mock_component>();
        window_.reset(new munin::window(content_));
    }
    
    std::shared_ptr<mock_component> content_;
    std::unique_ptr<munin::window>  window_;
};

TEST_F(a_new_window, sets_the_size_of_the_content_when_having_its_size_set)
{
    auto const size = terminalpp::extent({80, 24});
    EXPECT_CALL(*content_, do_set_size(size));
    
    window_->set_size(size);
}
