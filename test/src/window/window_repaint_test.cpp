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
      : canvas_(window_size),
        canvas_view_(canvas_),
        context_(canvas_view_)
    {
    }
    
    void SetUp()
    {
        reset_canvas(canvas_);
        
        ON_CALL(*content_, do_set_size(_))
            .WillByDefault(Invoke(
                [this](auto const &size)
                {
                    content_->on_redraw({{{}, size}});
                }));
                
        ON_CALL(*content_, do_draw(_, _))
            .WillByDefault(WithArgs<1>(Invoke(
                [this](auto const &region)
                {
                    this->increment_elements_within(region);
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
                printf("inc %d, %d = %d\n", x, y, canvas_[x][y].glyph_.character_);
            }
        }
    }
    
    static constexpr terminalpp::extent const window_size {20, 40};
    
    std::shared_ptr<mock_component> content_ = std::make_shared<mock_component>();
    std::shared_ptr<munin::window> window_ = std::make_shared<munin::window>(content_);
    terminalpp::canvas canvas_;
    terminalpp::canvas_view canvas_view_;
    munin::context context_;
};

constexpr terminalpp::extent const repainting_a_window::window_size;

TEST_F(repainting_a_window, of_size_zero_does_not_paint_any_data)
{
    window_->repaint(context_);
    
    for (auto y = 0; y < window_size.height; ++y)
    {
        for (auto x = 0; x < window_size.width; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_change_of_size_repaints_entire_canvas)
{
    window_->set_size(window_size);
    window_->repaint(context_);
    
    for (auto y = 0; y < window_size.height; ++y)
    {
        for (auto x = 0; x < window_size.width; ++x)
        {
            ASSERT_EQ(1, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_repaint_request_of_zero_size_repaints_nothing)
{
    window_->set_size(window_size);
    window_->repaint(context_);
    reset_canvas(canvas_);
    
    content_->on_redraw({{{}, {}}});
    window_->repaint(context_);
    
    for (auto y = 0; y < window_size.height; ++y)
    {
        for (auto x = 0; x < window_size.width; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_repaint_with_one_region_repaints_only_that_region)
{
    window_->set_size(window_size);
    window_->repaint(context_);
    reset_canvas(canvas_);
    
    content_->on_redraw({{{}, {window_size.width, 1}}});
    window_->repaint(context_);

    for (auto x = 0; x < window_size.width; ++x)
    {
        ASSERT_EQ(1, canvas_[x][0].glyph_.character_);
    }

    for (auto y = 1; y < window_size.height; ++y)
    {
        for (auto x = 0; x < window_size.width; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_repaint_with_two_discrete_regions_repaints_only_those_regions)
{
    window_->set_size(window_size);
    window_->repaint(context_);
    reset_canvas(canvas_);
    
    content_->on_redraw({
        {{}, {window_size.width, 1}},
        {{0, 1}, {window_size.width, 1}}
    });
    window_->repaint(context_);

    for (auto x = 0; x < window_size.width; ++x)
    {
        ASSERT_EQ(1, canvas_[x][0].glyph_.character_);
        ASSERT_EQ(1, canvas_[x][1].glyph_.character_);
    }

    for (auto y = 2; y < window_size.height; ++y)
    {
        for (auto x = 0; x < window_size.width; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}
