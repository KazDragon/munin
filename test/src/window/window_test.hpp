#pragma once

#include "mock/component.hpp"
#include <munin/window.hpp>
#include <memory>

class a_window : public testing::Test
{
protected:
    a_window()
      : window_(new munin::window(content_))
    {
    }
    
    std::shared_ptr<mock_component> content_ = std::make_shared<mock_component>();
    std::unique_ptr<munin::window>  window_;
};
