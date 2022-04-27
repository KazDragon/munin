#pragma once

#include "mock/component.hpp"
#include <terminalpp/terminal.hpp>
#include <munin/window.hpp>
#include <memory>

class a_window_test_base
{
protected:
    a_window_test_base()
      : window_(new munin::window(content_)),
        terminal_{
            [](terminalpp::tokens) { FAIL(); },
            [this](terminalpp::bytes data) { result_.append(data.begin(), data.end()); }
        }
    {
    }
    
    std::shared_ptr<mock_component> content_ { make_mock_component() };
    std::unique_ptr<munin::window>  window_;

    terminalpp::terminal terminal_;
    terminalpp::byte_storage result_;
};

class a_window : 
    public a_window_test_base,
    public testing::Test
{
};
