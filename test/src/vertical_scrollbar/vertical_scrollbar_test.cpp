#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "mock/component.hpp"
#include "mock/render_surface_capabilities.hpp"
#include "redraw.hpp"
#include <munin/detail/border_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <munin/vertical_scrollbar.hpp>
#include <terminalpp/mouse.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT
using testing::Return;
using testing::ValuesIn;

namespace {

auto const single_lined_vertical_beam =
    terminalpp::string{munin::detail::single_lined_vertical_beam};
auto const single_lined_cross =
    terminalpp::string{munin::detail::single_lined_cross};

class a_vertical_scrollbar : public testing::Test
{
 protected:
  std::shared_ptr<munin::vertical_scrollbar> scrollbar_{
      munin::make_vertical_scrollbar()};
};

using a_new_vertical_scrollbar = a_vertical_scrollbar;

}  // namespace

TEST_F(a_new_vertical_scrollbar, has_a_1x0_preferred_size)
{
  ASSERT_EQ(terminalpp::extent(1, 0), scrollbar_->get_preferred_size());
}

TEST_F(a_new_vertical_scrollbar, draws_nothing)
{
  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  scrollbar_->draw(surface);

  assert_similar_canvas_block(
      {
          // clang-format off
          "XXXX"_ts,
          "XXXX"_ts,
          "XXXX"_ts,
          "XXXX"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(a_vertical_scrollbar, has_a_preferred_size_relative_to_its_height)
{
  scrollbar_->set_size({4, 4});
  ASSERT_EQ(terminalpp::extent(1, 4), scrollbar_->get_preferred_size());

  scrollbar_->set_size({7, 3});
  ASSERT_EQ(terminalpp::extent(1, 3), scrollbar_->get_preferred_size());
}

TEST_F(a_vertical_scrollbar, with_size_but_no_slider_draws_a_frame_border)
{
  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});

  munin::render_surface surface{canvas};
  scrollbar_->draw(surface);

  assert_similar_canvas_block(
      {
          // clang-format off
          single_lined_vertical_beam + "XXX"_ts,
          single_lined_vertical_beam + "XXX"_ts,
          single_lined_vertical_beam + "XXX"_ts,
          single_lined_vertical_beam + "XXX"_ts,
          // clang-format on
      },
      canvas);
}

namespace {

using slider_test_data = std::tuple<
    terminalpp::coordinate_type,  // y_position
    terminalpp::coordinate_type,  // height
    terminalpp::coordinate_type   // expected x co-ordinate of slider
    >;

class vertical_scroll_bar_slider_position
  : public testing::TestWithParam<slider_test_data>
{
 protected:
  vertical_scroll_bar_slider_position()
  {
    scrollbar_->set_size(canvas_.size());
    fill_canvas(canvas_, 'X');

    scrollbar_->on_redraw.connect(
        redraw_component_on_surface(*scrollbar_, surface_));
  }

  std::shared_ptr<munin::vertical_scrollbar> scrollbar_{
      munin::make_vertical_scrollbar()};

  terminalpp::canvas canvas_{{1, 16}};
  mock_render_surface_capabilities capabilities_;
  munin::render_surface surface_{canvas_, capabilities_};
};

}  // namespace

TEST_P(
    vertical_scroll_bar_slider_position,
    draws_unicode_slider_at_the_expected_position)
{
  ON_CALL(capabilities_, supports_unicode()).WillByDefault(Return(true));

  using std::get;

  auto const &param = GetParam();
  auto const &y_position = get<0>(param);
  auto const &height = get<1>(param);
  auto const &expected_slider_y_position = get<2>(param);

  scrollbar_->set_slider_position(y_position, height);

  for (terminalpp::coordinate_type y = 0; y < canvas_.size().height_; ++y)
  {
    if (y == expected_slider_y_position)
    {
      ASSERT_EQ(munin::detail::border::unicode::cross, canvas_[0][y])
          << "Expected slider cross at position " << y;
    }
    else
    {
      ASSERT_EQ(munin::detail::border::unicode::vertical_beam, canvas_[0][y])
          << "Expected beam at position " << y;
    }
  }
}

TEST_P(
    vertical_scroll_bar_slider_position,
    draws_ansi_slider_at_the_expected_position)
{
  ON_CALL(capabilities_, supports_unicode()).WillByDefault(Return(false));

  using std::get;

  auto const &param = GetParam();
  auto const &y_position = get<0>(param);
  auto const &height = get<1>(param);
  auto const &expected_slider_y_position = get<2>(param);

  scrollbar_->set_slider_position(y_position, height);

  for (terminalpp::coordinate_type y = 0; y < canvas_.size().height_; ++y)
  {
    if (y == expected_slider_y_position)
    {
      ASSERT_EQ(munin::detail::border::ansi::cross, canvas_[0][y])
          << "Expected slider cross at position " << y;
    }
    else
    {
      ASSERT_EQ(munin::detail::border::ansi::vertical_beam, canvas_[0][y])
          << "Expected beam at position " << y;
    }
  }
}

static slider_test_data const test_data[] = {
    slider_test_data{0, 20, 0},  // Special: 0
    slider_test_data{1, 20, 1},  // 1
    slider_test_data{2, 20, 1},  // 1 + 14/18 // (14 / 18 per step )
    slider_test_data{3, 20, 2},  // 2 + 10/18
    slider_test_data{4, 20, 3},  // 3 + 6/18

    slider_test_data{5, 20, 4},  // 4 + 2/18
    slider_test_data{6, 20, 4},  // 4 + 16/18
    slider_test_data{7, 20, 5},  // 5 + 12/18
    slider_test_data{8, 20, 6},  // 6 + 8/18
    slider_test_data{9, 20, 7},  // 7 + 4/18

    slider_test_data{10, 20, 8},   // 8 + 0/18
    slider_test_data{11, 20, 8},   // 8 + 14/18
    slider_test_data{12, 20, 9},   // 9 + 10/18
    slider_test_data{13, 20, 10},  // 10 + 6/18
    slider_test_data{14, 20, 11},  // 11 + 2/18

    slider_test_data{15, 20, 11},  // 11 + 16/18
    slider_test_data{16, 20, 12},  // 12 + 12/18
    slider_test_data{17, 20, 13},  // 13 + 8/18
    slider_test_data{18, 20, 14},  // 14 + 4/18

    slider_test_data{19, 20, 15},  // Special: 15
};

INSTANTIATE_TEST_SUITE_P(
    setting_slider_position_shows_slider_in_expected_location,
    vertical_scroll_bar_slider_position,
    ValuesIn(test_data));

TEST_F(a_vertical_scrollbar, draws_the_correct_scroller_position_when_resized)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});
  scrollbar_->set_slider_position(75, 100);

  munin::render_surface surface{canvas};
  scrollbar_->draw(surface);

  assert_similar_canvas_block(
      {
          // clang-format off
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          single_lined_cross,
          single_lined_vertical_beam,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          // clang-format on
      },
      canvas);

  scrollbar_->set_size({1, 8});
  scrollbar_->draw(surface);

  assert_similar_canvas_block(
      {
          // clang-format off
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          single_lined_cross,
          single_lined_vertical_beam,
          single_lined_vertical_beam,
          // clang-format on
      },
      canvas);
}

namespace {

constexpr auto lowlight_attribute =
    terminalpp::attribute{terminalpp::greyscale_colour{5}};

auto const lowlight_vertical_beam = terminalpp::string{
    {munin::detail::single_lined_vertical_beam, lowlight_attribute}};

constexpr auto highlight_attribute =
    terminalpp::attribute{terminalpp::high_colour{4, 5, 1}};

auto const highlight_vertical_beam = terminalpp::string{
    {munin::detail::single_lined_vertical_beam, highlight_attribute}};

class a_vertical_scrollbar_with_an_associated_component
  : public a_vertical_scrollbar
{
 public:
  a_vertical_scrollbar_with_an_associated_component()
  {
    scrollbar_->highlight_on_focus(associated_component_);
    scrollbar_->set_lowlight_attribute(lowlight_attribute);
    scrollbar_->set_highlight_attribute(highlight_attribute);
  }

 protected:
  std::shared_ptr<mock_component> associated_component_{make_mock_component()};
};

}  // namespace

TEST_F(
    a_vertical_scrollbar_with_an_associated_component,
    draws_the_lowlight_attribute_by_default)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 8});

  munin::render_surface surface{canvas};
  scrollbar_->draw(surface);

  assert_similar_canvas_block(
      {
          // clang-format off
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_vertical_scrollbar_with_an_associated_component,
    draws_the_highlight_attribute_after_the_associated_component_gains_focus)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 8});

  munin::render_surface surface{canvas};

  scrollbar_->on_redraw.connect(
      redraw_component_on_surface(*scrollbar_, surface));

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  assert_similar_canvas_block(
      {
          // clang-format off
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_vertical_scrollbar_with_an_associated_component,
    draws_the_lowlight_attribute_after_the_associated_component_loses_focus)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});

  munin::render_surface surface{canvas};
  scrollbar_->on_redraw.connect(
      redraw_component_on_surface(*scrollbar_, surface));

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(false));
  associated_component_->on_focus_lost();

  assert_similar_canvas_block(
      {
          // clang-format off
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          lowlight_vertical_beam,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_vertical_scrollbar,
    draws_the_highlight_attribute_when_associated_with_a_focused_component)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});

  munin::render_surface surface{canvas};
  scrollbar_->on_redraw.connect(
      redraw_component_on_surface(*scrollbar_, surface));

  auto associated_component = make_mock_component();
  ON_CALL(*associated_component, do_has_focus()).WillByDefault(Return(true));

  scrollbar_->set_lowlight_attribute(lowlight_attribute);
  scrollbar_->set_highlight_attribute(highlight_attribute);
  scrollbar_->highlight_on_focus(associated_component);

  assert_similar_canvas_block(
      {
          // clang-format off
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          highlight_vertical_beam,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_vertical_scrollbar_with_an_associated_component,
    draws_the_new_lowlight_attribute_when_it_is_changed)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});

  munin::render_surface surface{canvas};
  scrollbar_->on_redraw.connect(
      redraw_component_on_surface(*scrollbar_, surface));

  constexpr auto new_lowlight_attribute =
      terminalpp::attribute{terminalpp::high_colour(2, 2, 2)};

  auto const new_lowlight_vertical_beam = terminalpp::string{
      {munin::detail::single_lined_vertical_beam, new_lowlight_attribute}};

  scrollbar_->set_lowlight_attribute(new_lowlight_attribute);

  assert_similar_canvas_block(
      {
          // clang-format off
          new_lowlight_vertical_beam,
          new_lowlight_vertical_beam,
          new_lowlight_vertical_beam,
          new_lowlight_vertical_beam,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(
    a_vertical_scrollbar_with_an_associated_component,
    draws_the_new_highlight_attribute_when_it_is_changed)
{
  terminalpp::canvas canvas({1, 8});
  fill_canvas(canvas, 'X');

  scrollbar_->set_size({1, 4});

  munin::render_surface surface{canvas};
  scrollbar_->on_redraw.connect(
      redraw_component_on_surface(*scrollbar_, surface));

  constexpr auto new_highlight_attribute =
      terminalpp::attribute{terminalpp::high_colour(6, 6, 5)};

  auto const new_highlight_vertical_beam = terminalpp::string{
      {munin::detail::single_lined_vertical_beam, new_highlight_attribute}};

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  scrollbar_->set_highlight_attribute(new_highlight_attribute);

  assert_similar_canvas_block(
      {
          // clang-format off
          new_highlight_vertical_beam,
          new_highlight_vertical_beam,
          new_highlight_vertical_beam,
          new_highlight_vertical_beam,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          "X"_ts,
          // clang-format on
      },
      canvas);
}

TEST_F(a_vertical_scrollbar, with_no_slider_emits_no_scroll_events_when_clicked)
{
  scrollbar_->set_size({1, 5});

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {1, 0}};

  bool scroll_up_clicked = false;
  bool scroll_down_clicked = false;

  scrollbar_->on_scroll_up.connect([&] { scroll_up_clicked = true; });

  scrollbar_->on_scroll_down.connect([&] { scroll_down_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_up_clicked);
  ASSERT_FALSE(scroll_down_clicked);
}

TEST_F(
    a_vertical_scrollbar,
    with_a_slider_emits_scroll_up_when_clicked_above_the_slider)
{
  scrollbar_->set_size({1, 5});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {0, 0}};

  bool scroll_up_clicked = false;
  bool scroll_down_clicked = false;

  scrollbar_->on_scroll_up.connect([&] { scroll_up_clicked = true; });

  scrollbar_->on_scroll_down.connect([&] { scroll_down_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_TRUE(scroll_up_clicked);
  ASSERT_FALSE(scroll_down_clicked);
}

TEST_F(
    a_vertical_scrollbar,
    with_a_slider_emits_scroll_down_when_clicked_below_the_slider)
{
  scrollbar_->set_size({1, 5});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {0, 4}};

  bool scroll_up_clicked = false;
  bool scroll_down_clicked = false;

  scrollbar_->on_scroll_up.connect([&] { scroll_up_clicked = true; });

  scrollbar_->on_scroll_down.connect([&] { scroll_down_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_up_clicked);
  ASSERT_TRUE(scroll_down_clicked);
}

TEST_F(
    a_vertical_scrollbar,
    with_a_slider_emits_nothing_when_clicked_on_the_slider)
{
  scrollbar_->set_size({1, 5});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {0, 2}};

  bool scroll_up_clicked = false;
  bool scroll_down_clicked = false;

  scrollbar_->on_scroll_up.connect([&] { scroll_up_clicked = true; });

  scrollbar_->on_scroll_down.connect([&] { scroll_down_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_up_clicked);
  ASSERT_FALSE(scroll_down_clicked);
}