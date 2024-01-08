#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "mock/animator.hpp"
#include "redraw.hpp"
#include <munin/render_surface.hpp>
#include <munin/status_bar.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT
using namespace std::literals;         // NOLINT
using testing::_;                      // NOLINT
using testing::Return;

TEST(a_status_bar, is_a_component)
{
  std::shared_ptr<mock_animator> animator{make_mock_animator()};
  std::shared_ptr<munin::component> comp = munin::make_status_bar(*animator);
}

namespace {

class a_new_status_bar : public testing::Test
{
 protected:
  std::shared_ptr<mock_animator> animator_{make_mock_animator()};
  std::shared_ptr<munin::status_bar> status_bar_{
      munin::make_status_bar(*animator_)};
};

}  // namespace

TEST_F(a_new_status_bar, has_a_1x1_preferred_size)
{
  ASSERT_EQ(terminalpp::extent(1, 1), status_bar_->get_preferred_size());
}

TEST_F(a_new_status_bar, draws_itself_as_blank)
{
  terminalpp::canvas canvas{{5, 3}};
  fill_canvas(canvas, 'x');

  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});

  status_bar_->set_size({3, 1});
  status_bar_->draw(surface, {{}, status_bar_->get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxx"_ts,
          "x   x"_ts,
          "xxxxx"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_new_status_bar, has_a_preferred_size_of_the_message_when_a_message_is_set)
{
  static auto const message = "Test!"_ts;

  std::optional<terminalpp::extent> preferred_size;
  status_bar_->on_preferred_size_changed.connect(
      [&preferred_size, this]()
      { preferred_size = status_bar_->get_preferred_size(); });

  status_bar_->set_message(message);

  auto const expected_size = terminalpp::extent{
      static_cast<terminalpp::coordinate_type>(message.size()), 1};

  ASSERT_TRUE(preferred_size.has_value());
  ASSERT_EQ(expected_size, *preferred_size);
}

TEST_F(a_new_status_bar, redraws_the_message_when_a_message_is_set)
{
  static auto const message = "Test!"_ts;

  terminalpp::canvas canvas{{7, 3}};
  fill_canvas(canvas, 'x');

  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});

  status_bar_->set_size({5, 1});

  std::vector<terminalpp::rectangle> redraw_regions;
  status_bar_->on_redraw.connect(append_regions_to_container(redraw_regions));

  status_bar_->set_message(message);

  for (auto const &redraw_region : redraw_regions)
  {
    status_bar_->draw(surface, redraw_region);
  }

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxxxx"_ts,
          "xTest!x"_ts,
          "xxxxxxx"_ts,
          // clang-format on
      },
      canvas);
}

namespace {

class an_animated_status_bar : public testing::Test
{
 public:
  an_animated_status_bar() : time_0_(std::chrono::steady_clock::now())
  {
    ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_));

    status_bar_->on_redraw.connect(
        append_regions_to_container(redraw_regions_));

    fill_canvas(canvas_, 'x');
    surface_.offset_by({1, 1});

    status_bar_->set_size({5, 1});
    status_bar_->set_message("Test!"_ts);
    redraw_requested_regions();
  }

  [[nodiscard]] auto number_of_requested_regions() const
  {
    return redraw_regions_.size();
  }

 protected:
  void redraw_requested_regions()
  {
    for (auto const &redraw_region : redraw_regions_)
    {
      status_bar_->draw(surface_, redraw_region);
    }

    redraw_regions_.clear();
  }

  std::shared_ptr<mock_animator> animator_{make_mock_animator()};
  std::shared_ptr<munin::status_bar> status_bar_{
      munin::make_status_bar(*animator_)};

  std::chrono::steady_clock::time_point const time_0_;

  terminalpp::canvas canvas_{{7, 3}};
  munin::render_surface surface_{canvas_};

 private:
  std::vector<terminalpp::rectangle> redraw_regions_;
};

}  // namespace

TEST_F(
    an_animated_status_bar,
    less_than_5_seconds_after_drawing_a_message_still_draws_that_message)
{
  ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_ + 5s - 1ms));
  animator_->redraw_components();

  redraw_requested_regions();

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxxxx"_ts,
          "xTest!x"_ts,
          "xxxxxxx"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(an_animated_status_bar, after_5_seconds_marquees_two_characters_off)
{
  ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_ + 5s));
  animator_->redraw_components();

  redraw_requested_regions();

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxxxx"_ts,
          "xst!  x"_ts,
          "xxxxxxx"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(an_animated_status_bar, after_5015ms_marquees_four_characters_off)
{
  ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_ + 5015ms));
  animator_->redraw_components();

  redraw_requested_regions();

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxxxx"_ts,
          "x!    x"_ts,
          "xxxxxxx"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(an_animated_status_bar, after_5030ms_marquees_all_characters_off)
{
  ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_ + 5030s));
  animator_->redraw_components();

  redraw_requested_regions();

  assert_similar_canvas_block(
      {
          // clang-format off
          "xxxxxxx"_ts,
          "x     x"_ts,
          "xxxxxxx"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(
    an_animated_status_bar,
    after_drawing_all_of_its_animation_frames_does_not_request_a_redraw)
{
  // At time 0, the screen is freshly painted and a delayed function is
  // waiting for execution, but no regions are requested.
  ASSERT_EQ(0, number_of_requested_regions());

  // After updating the animation time to after the last frame, the
  // function is executed and there are no more delayed functions, and
  // there are redraw requests awaiting.
  ON_CALL(*animator_, do_now()).WillByDefault(Return(time_0_ + 6s));
  animator_->redraw_components();

  ASSERT_GT(number_of_requested_regions(), 0);

  // After redrawing the requested regions for the final frame, there are
  // no delayed functions or redraw requests.
  animator_->redraw_components();

  redraw_requested_regions();
  ASSERT_EQ(0, number_of_requested_regions());
}
