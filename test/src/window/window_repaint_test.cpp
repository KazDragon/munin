#include "fill_canvas.hpp"
#include "window_test.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/screen.hpp>
#include <terminalpp/terminal.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT
using testing::_;                      // NOLINT
using testing::InSequence;
using testing::Return;
using testing::ReturnPointee;
using testing::SaveArg;
using testing::WithArgs;

TEST_F(a_window, requests_a_repaint_when_content_requests_a_redraw)
{
  int repaint_request_called = 0;

  window_->on_repaint_request.connect([&] { ++repaint_request_called; });

  content_->on_redraw({{{}, {}}});

  ASSERT_EQ(1, repaint_request_called);
}

TEST_F(
    a_window,
    does_not_repeatedly_request_repaints_when_content_requests_redraws)
{
  int repaint_request_called = 0;

  window_->on_repaint_request.connect([&] { ++repaint_request_called; });

  content_->on_redraw({{{}, {}}});
  content_->on_redraw({{{}, {}}});

  ASSERT_EQ(1, repaint_request_called);
}

TEST_F(
    a_window,
    again_requests_repaint_when_content_requests_a_redraw_after_a_paint)
{
  auto const window_size = terminalpp::extent{20, 30};
  int repaint_request_called = 0;

  window_->on_repaint_request.connect([&] { ++repaint_request_called; });

  ON_CALL(*content_, do_get_size()).WillByDefault(Return(window_size));

  content_->on_redraw({{{}, {}}});

  terminalpp::canvas canvas(window_size);
  window_->repaint(canvas);

  content_->on_redraw({{{}, {}}});

  ASSERT_EQ(2, repaint_request_called);
}

class repainting_a_window : public a_window_test_base, public testing::Test
{
 protected:
  repainting_a_window() : canvas_(window_size)
  {
    fill_canvas(canvas_, 0);

    ON_CALL(*content_, do_draw(_, _))
        .WillByDefault(
            WithArgs<1>([this](auto const &region)
                        { this->increment_elements_within(region); }));

    // TODO: mock impl of set/get size
    ON_CALL(*content_, do_set_size(_))
        .WillByDefault(SaveArg<0>(&content_size_));

    ON_CALL(*content_, do_get_size())
        .WillByDefault(ReturnPointee(&content_size_));
  }

  void increment_elements_within(terminalpp::rectangle const &region)
  {
    terminalpp::for_each_in_region(
        canvas_,
        region,
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,  // NOLINT
           terminalpp::coordinate_type row) { ++elem.glyph_.character_; });
  }

  static constexpr terminalpp::extent const window_size{20, 40};

  terminalpp::canvas canvas_;
  terminalpp::extent content_size_;
};

TEST_F(repainting_a_window, for_the_first_time_sets_component_size)
{
  EXPECT_CALL(*content_, do_set_size(window_size));
  window_->repaint(canvas_);
}

TEST_F(
    repainting_a_window, with_the_same_canvas_size_does_not_change_content_size)
{
  EXPECT_CALL(*content_, do_set_size(window_size)).Times(1);
  window_->repaint(canvas_);
  window_->repaint(canvas_);
}

TEST_F(
    repainting_a_window, with_a_differently_sized_canvas_changes_content_size)
{
  static constexpr terminalpp::extent new_size{
      window_size.width_ + 1, window_size.height_ + 1};

  {
    InSequence s;
    EXPECT_CALL(*content_, do_set_size(window_size))
        .WillOnce(SaveArg<0>(&content_size_));
    EXPECT_CALL(*content_, do_set_size(new_size))
        .WillOnce(SaveArg<0>(&content_size_));
  }

  window_->repaint(canvas_);

  canvas_ = terminalpp::canvas(new_size);
  window_->repaint(canvas_);
}

TEST_F(repainting_a_window, after_a_change_of_size_repaints_entire_canvas)
{
  static constexpr terminalpp::extent new_size{
      window_size.width_ + 1, window_size.height_ + 1};

  window_->repaint(canvas_);

  canvas_ = terminalpp::canvas(new_size);
  fill_canvas(canvas_, 0);

  window_->repaint(canvas_);

  terminalpp::for_each_in_region(
      canvas_,
      {{}, window_size},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,  // NOLINT
         terminalpp::coordinate_type row)
      {
        ASSERT_EQ(1, elem.glyph_.character_)
            << "row = " << row << ", column = " << column;
      });
}

TEST_F(
    repainting_a_window,
    after_a_repaint_with_one_region_repaints_only_that_region)
{
  window_->repaint(canvas_);
  fill_canvas(canvas_, 0);

  content_->on_redraw({{{}, {window_size.width_, 1}}});
  window_->repaint(canvas_);

  terminalpp::for_each_in_region(
      canvas_,
      {{}, {window_size.width_, 1}},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,  // NOLINT
         terminalpp::coordinate_type row)
      {
        ASSERT_EQ(1, elem.glyph_.character_)
            << "row = " << row << ", column = " << column;
      });

  terminalpp::for_each_in_region(
      canvas_,
      {{0, 1}, {window_size.width_, window_size.height_ - 1}},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,  // NOLINT
         terminalpp::coordinate_type row)
      {
        ASSERT_EQ(0, elem.glyph_.character_)
            << "row = " << row << ", column = " << column;
      });
}

TEST_F(
    repainting_a_window,
    after_a_repaint_with_two_discrete_regions_repaints_only_those_regions)
{
  window_->repaint(canvas_);
  fill_canvas(canvas_, 0);

  content_->on_redraw(
      {{{}, {window_size.width_, 1}}, {{0, 1}, {window_size.width_, 1}}});
  window_->repaint(canvas_);

  terminalpp::for_each_in_region(
      canvas_,
      {{}, {window_size.width_, 2}},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,  // NOLINT
         terminalpp::coordinate_type row)
      {
        ASSERT_EQ(1, elem.glyph_.character_)
            << "row = " << row << ", column = " << column;
      });

  terminalpp::for_each_in_region(
      canvas_,
      {{0, 2}, {window_size.width_, window_size.height_ - 2}},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,  // NOLINT
         terminalpp::coordinate_type row)
      {
        ASSERT_EQ(0, elem.glyph_.character_)
            << "row = " << row << ", column = " << column;
      });
}

TEST_F(repainting_a_window, with_no_changes_returns_empty_paint_data)
{
  window_->repaint(canvas_);

  content_->on_redraw({{{}, {}}});
  channel_.written.clear();

  window_->repaint(canvas_);

  auto const expected_repaint_output = ""_tb;
  ASSERT_EQ(expected_repaint_output, channel_.written);
}
