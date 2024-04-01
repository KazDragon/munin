#pragma once

#include "fake/channel.hpp"
#include "mock/component.hpp"

#include <munin/window.hpp>
#include <terminalpp/terminal.hpp>

#include <memory>

class a_window_test_base
{
protected:
    a_window_test_base() : window_(new munin::window(terminal_, content_))
    {
    }

    fake_channel channel_;
    terminalpp::terminal terminal_{channel_};

    std::shared_ptr<mock_component> content_{make_mock_component()};
    std::unique_ptr<munin::window> window_;
};

class a_window : public a_window_test_base, public testing::Test
{
};
