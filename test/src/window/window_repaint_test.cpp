#include "window_test.hpp"
#include <terminalpp/canvas.hpp>
#include <terminalpp/screen.hpp>
#include <terminalpp/terminal.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;

using testing::InSequence;
using testing::Return;
using testing::ReturnPointee;
using testing::SaveArg;
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
    auto const window_size = terminalpp::extent{20, 30};
    int repaint_request_called = 0;

    window_->on_repaint_request.connect([&]
    {
        ++repaint_request_called;
    });
 
    ON_CALL(*content_, do_get_size())
        .WillByDefault(Return(window_size));
    
    content_->on_redraw({{{}, {}}});
    
    terminalpp::canvas canvas(window_size);
    terminalpp::terminal terminal;
    window_->repaint(canvas, terminal, discard_result);
    
    content_->on_redraw({{{}, {}}});

    ASSERT_EQ(2, repaint_request_called);
}

class repainting_a_window : 
    public a_window_test_base,
    public testing::Test
{
protected :
    repainting_a_window()
      : canvas_(window_size)
    {
        reset_canvas(canvas_);
        
        ON_CALL(*content_, do_draw(_, _))
            .WillByDefault(WithArgs<1>(
                [this](auto const &region)
                {
                    this->increment_elements_within(region);
                }));
               
               // TODO: mock impl of set/get size 
        ON_CALL(*content_, do_set_size(_))
            .WillByDefault(SaveArg<0>(&content_size_));
                
        ON_CALL(*content_, do_get_size())
            .WillByDefault(ReturnPointee(&content_size_));
    }
    
    void reset_canvas(terminalpp::canvas &cvs)
    {
        auto const canvas_size = cvs.size();
        
        for (auto row = 0; row < canvas_size.height_; ++row)
        {
            for (auto col = 0; col < canvas_size.width_; ++col)
            {
                cvs[col][row].glyph_.character_ = 0;
            }
        }
    }
    
    void increment_elements_within(terminalpp::rectangle const &region)
    {
        for (auto y = region.origin_.y_; y < region.origin_.y_ + region.size_.height_; ++y)
        {
            for (auto x = region.origin_.x_; x < region.origin_.x_ + region.size_.width_; ++x)
            {
                ++canvas_[x][y].glyph_.character_;
            }
        }
    }
    
    static constexpr terminalpp::extent const window_size {20, 40};
    
    terminalpp::terminal terminal_;
    terminalpp::canvas canvas_;
    terminalpp::extent content_size_;

    std::function<void (terminalpp::bytes)> do_repaint_ =
        [this](terminalpp::bytes data)
        {
            repaint_output_.append(data.cbegin(), data.cend());
        };

    terminalpp::byte_storage repaint_output_;
};

constexpr terminalpp::extent const repainting_a_window::window_size;

TEST_F(repainting_a_window, for_the_first_time_sets_component_size)
{
    EXPECT_CALL(*content_, do_set_size(window_size));
    window_->repaint(canvas_, terminal_, do_repaint_);
}

TEST_F(repainting_a_window, with_the_same_canvas_size_does_not_change_content_size)
{
    EXPECT_CALL(*content_, do_set_size(window_size))
        .Times(1);
    window_->repaint(canvas_, terminal_, do_repaint_);
    window_->repaint(canvas_, terminal_, do_repaint_);
}

TEST_F(repainting_a_window, with_a_differently_sized_canvas_changes_content_size)
{
    static constexpr terminalpp::extent new_size{
        window_size.width_ + 1, window_size.height_ + 1
    };
    
    {
        InSequence s;
        EXPECT_CALL(*content_, do_set_size(window_size))
            .WillOnce(SaveArg<0>(&content_size_));
        EXPECT_CALL(*content_, do_set_size(new_size))
            .WillOnce(SaveArg<0>(&content_size_));
    }
            
    window_->repaint(canvas_, terminal_, do_repaint_);
    
    canvas_ = terminalpp::canvas(new_size);
    window_->repaint(canvas_, terminal_, do_repaint_);
}

TEST_F(repainting_a_window, after_a_change_of_size_repaints_entire_canvas)
{
    static constexpr terminalpp::extent new_size{
        window_size.width_ + 1, window_size.height_ + 1
    };
    
    window_->repaint(canvas_, terminal_, do_repaint_);
    
    canvas_ = terminalpp::canvas(new_size);
    reset_canvas(canvas_);
    
    window_->repaint(canvas_, terminal_, do_repaint_);

    for (auto y = 0; y < window_size.height_; ++y)
    {
        for (auto x = 0; x < window_size.width_; ++x)
        {
            ASSERT_EQ(1, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_repaint_with_one_region_repaints_only_that_region)
{
    window_->repaint(canvas_, terminal_, do_repaint_);
    reset_canvas(canvas_);
    
    content_->on_redraw({{{}, {window_size.width_, 1}}});
    window_->repaint(canvas_, terminal_, do_repaint_);

    for (auto x = 0; x < window_size.width_; ++x)
    {
        ASSERT_EQ(1, canvas_[x][0].glyph_.character_);
    }

    for (auto y = 1; y < window_size.height_; ++y)
    {
        for (auto x = 0; x < window_size.width_; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, after_a_repaint_with_two_discrete_regions_repaints_only_those_regions)
{
    window_->repaint(canvas_, terminal_, do_repaint_);
    reset_canvas(canvas_);
    
    content_->on_redraw({
        {{}, {window_size.width_, 1}},
        {{0, 1}, {window_size.width_, 1}}
    });
    window_->repaint(canvas_, terminal_, do_repaint_);

    for (auto x = 0; x < window_size.width_; ++x)
    {
        ASSERT_EQ(1, canvas_[x][0].glyph_.character_);
        ASSERT_EQ(1, canvas_[x][1].glyph_.character_);
    }

    for (auto y = 2; y < window_size.height_; ++y)
    {
        for (auto x = 0; x < window_size.width_; ++x)
        {
            ASSERT_EQ(0, canvas_[x][y].glyph_.character_);
        }
    }
}

TEST_F(repainting_a_window, with_no_changes_returns_empty_paint_data)
{
    window_->repaint(canvas_, terminal_, do_repaint_);

    content_->on_redraw({{{}, {}}});

    repaint_output_.clear();
    window_->repaint(canvas_, terminal_, do_repaint_);

    auto const expected_repaint_output = ""_tb;
    ASSERT_EQ(expected_repaint_output, repaint_output_);
}
