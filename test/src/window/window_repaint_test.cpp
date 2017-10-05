#include "window_test.hpp"
#include <munin/context.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>

using testing::Invoke;
using testing::WithArgs;
using testing::_;

TEST_F(a_window, requests_a_repaint_when_content_requests_a_redraw)
{
    int repaint_request_called = 0;
    
    window_->on_repaint_request.connect([&]
    {
        ++repaint_request_called;
    });
    
    content_->on_redraw({{{}, {}}});
    
    ASSERT_EQ(1, repaint_request_called);
}

TEST_F(a_window, does_not_repeatedly_request_repaints_when_content_requests_redraws)
{
    int repaint_request_called = 0;
    
    window_->on_repaint_request.connect([&]
    {
        ++repaint_request_called;
    });
    
    content_->on_redraw({{{}, {}}});
    content_->on_redraw({{{}, {}}});
    
    ASSERT_EQ(1, repaint_request_called);
}

TEST_F(a_window, again_requests_repaint_when_content_requests_a_redraw_after_a_paint)
{
    int repaint_request_called = 0;
    
    window_->on_repaint_request.connect([&]
    {
        ++repaint_request_called;
    });
    
    content_->on_redraw({{{}, {}}});

    terminalpp::canvas canvas({});
    terminalpp::canvas_view canvas_view(canvas);
    
    munin::context ctx(canvas_view);
    window_->repaint(ctx);
    
    content_->on_redraw({{{}, {}}});

    ASSERT_EQ(2, repaint_request_called);
}

class repainting_a_window : public testing::Test
{
protected :
    repainting_a_window()
      : canvas_(window_size)
    {
    }
    
    void SetUp()
    {
        reset_canvas(canvas_);
        
        ON_CALL(*content_, do_draw(_, _))
            .WillByDefault(WithArgs<1>(Invoke(
                [this](auto &region)
                {
                    increment_elements_within(region);
                })));
    }
    
    void reset_canvas(terminalpp::canvas &cvs)
    {
        auto const canvas_size = cvs.size();
        
        for (auto row = 0; row < canvas_size.height; ++row)
        {
            for (auto col = 0; col < canvas_size.width; ++col)
            {
                cvs[col][row].glyph_.character_ = 0;
            }
        }
    }
    
    void increment_elements_within(munin::rectangle const &region)
    {
        for (auto y = region.origin.y; y < region.origin.y + region.size.height; ++y)
        {
            for (auto x = region.origin.x; x < region.origin.x + region.size.width; ++x)
            {
                ++canvas_[x][y].glyph_.character_;
            }
        }
    }
    
    static constexpr terminalpp::extent const window_size {20, 40};
    
    std::shared_ptr<mock_component> content_ = std::make_shared<mock_component>();
    std::shared_ptr<munin::window> window_ = std::make_shared<munin::window>(content_);
    terminalpp::canvas canvas_;
};

terminalpp::extent const repainting_a_window::window_size;

TEST_F(repainting_a_window, of_size_zero_does_not_paint_any_data)
{
}

TEST_F(repainting_a_window, after_a_change_of_size_requests_a_complete_redraw)
{
    //terminalpp::canvas
}