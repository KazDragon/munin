#pragma once

#include "mock/component.hpp"
#include <munin/window.hpp>
#include <memory>

class a_window_test_base
{
protected:
    a_window_test_base()
      : window_(new munin::window(content_))
    {
    }
    
    std::shared_ptr<mock_component> content_ { make_mock_component() };
    std::unique_ptr<munin::window>  window_;

    terminalpp::byte_storage result_;
    std::function<void (terminalpp::bytes)> discard_result = 
        [](terminalpp::bytes)
        {
        };

    std::function<void (terminalpp::bytes)> append_to_result = 
        [this](terminalpp::bytes data)
        {
            result_.append(data.begin(), data.end());
        };
};

class a_window : 
    public a_window_test_base,
    public testing::Test
{
};
