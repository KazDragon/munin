#include "mock/component.hpp"
#include <munin/detail/unicode_glyphs.hpp>
#include <munin/horizontal_scrollbar.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <gtest/gtest.h>

using testing::Return;
using testing::ValuesIn;

namespace {

class a_horizontal_scrollbar : public testing::Test
{
 protected:
  std::shared_ptr<munin::horizontal_scrollbar> scrollbar_{
      munin::make_horizontal_scrollbar()};
};

using a_new_horizontal_scrollbar = a_horizontal_scrollbar;

}  // namespace

TEST_F(a_new_horizontal_scrollbar, has_a_0x1_preferred_size)
{
  ASSERT_EQ(terminalpp::extent(0, 1), scrollbar_->get_preferred_size());
}

TEST_F(a_new_horizontal_scrollbar, draws_nothing)
{
  terminalpp::canvas canvas({4, 4});
  munin::render_surface surface{canvas};

  terminalpp::for_each_in_region(
      canvas,
      {{}, canvas.size()},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,
         terminalpp::coordinate_type row) { elem = 'X'; });

  scrollbar_->draw(surface, {{0, 0}, {0, 0}});

  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][0]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][0]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][0]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][3]);
}

TEST_F(a_horizontal_scrollbar, has_a_preferred_size_relative_to_its_width)
{
  scrollbar_->set_size({4, 4});
  ASSERT_EQ(terminalpp::extent(4, 1), scrollbar_->get_preferred_size());

  scrollbar_->set_size({7, 3});
  ASSERT_EQ(terminalpp::extent(7, 1), scrollbar_->get_preferred_size());
}

TEST_F(a_horizontal_scrollbar, with_size_but_no_slider_draws_a_frame_border)
{
  terminalpp::canvas canvas({4, 4});
  munin::render_surface surface{canvas};

  terminalpp::for_each_in_region(
      canvas,
      {{}, canvas.size()},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,
         terminalpp::coordinate_type row) { elem = 'X'; });

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[3][0]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][1]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][2]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[1][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[2][3]);
  ASSERT_EQ(terminalpp::element{'X'}, canvas[3][3]);
}

namespace {

using slider_test_data = std::tuple<
    terminalpp::coordinate_type,  // x_position
    terminalpp::coordinate_type,  // width
    terminalpp::coordinate_type   // expected x co-ordinate of slider
    >;

class horizontal_scroll_bar_slider_position
  : public testing::TestWithParam<slider_test_data>
{
 protected:
  horizontal_scroll_bar_slider_position()
  {
    scrollbar_->set_size(canvas_.size());

    terminalpp::for_each_in_region(
        canvas_,
        {{}, canvas_.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row) { elem = 'X'; });

    scrollbar_->on_redraw.connect(
        [this](std::vector<terminalpp::rectangle> const &regions)
        {
          for (auto const &region : regions)
          {
            scrollbar_->draw(surface_, region);
          }
        });
  }

  std::shared_ptr<munin::horizontal_scrollbar> scrollbar_{
      munin::make_horizontal_scrollbar()};

  terminalpp::canvas canvas_{{16, 1}};
  munin::render_surface surface_{canvas_};
};

}  // namespace

TEST_P(
    horizontal_scroll_bar_slider_position,
    draws_slider_at_the_expected_position)
{
  using std::get;

  auto const &param = GetParam();
  auto const &x_position = get<0>(param);
  auto const &width = get<1>(param);
  auto const &expected_slider_x_position = get<2>(param);

  scrollbar_->set_slider_position(x_position, width);

  for (terminalpp::coordinate_type x = 0; x < canvas_.size().width_; ++x)
  {
    if (x == expected_slider_x_position)
    {
      ASSERT_EQ(munin::detail::single_lined_cross, canvas_[x][0])
          << "Expected slider cross at position " << x;
    }
    else
    {
      ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas_[x][0])
          << "Expected beam at position " << x;
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
    horizontal_scroll_bar_slider_position,
    ValuesIn(test_data));

TEST_F(a_horizontal_scrollbar, draws_the_correct_scroller_position_when_resized)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  terminalpp::for_each_in_region(
      canvas,
      {{}, canvas.size()},
      [](terminalpp::element &elem,
         terminalpp::coordinate_type column,
         terminalpp::coordinate_type row) { elem = 'X'; });

  scrollbar_->set_size({4, 1});
  scrollbar_->set_slider_position(75, 100);
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(munin::detail::single_lined_cross, canvas[2][0]);
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[3][0]);
  ASSERT_EQ('X', canvas[4][0]);
  ASSERT_EQ('X', canvas[5][0]);
  ASSERT_EQ('X', canvas[6][0]);
  ASSERT_EQ('X', canvas[7][0]);

  scrollbar_->set_size({8, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[0][0])
      << " expected beam on position 0";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[1][0])
      << " expected beam on position 1";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[2][0])
      << " expected beam on position 2";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[3][0])
      << " expected beam on position 3";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[4][0])
      << " expected beam on position 4";
  ASSERT_EQ(munin::detail::single_lined_cross, canvas[5][0])
      << " expected cross on position 5";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[6][0])
      << " expected beam on position 6";
  ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[7][0])
      << " expected beam on position 7";
}

namespace {

constexpr auto lowlight_attribute =
    terminalpp::attribute{terminalpp::greyscale_colour{5}};

constexpr auto lowlight_horizontal_beam = terminalpp::element{
    munin::detail::single_lined_horizontal_beam, lowlight_attribute};

constexpr auto highlight_attribute =
    terminalpp::attribute{terminalpp::high_colour{4, 5, 1}};

constexpr auto highlight_horizontal_beam = terminalpp::element{
    munin::detail::single_lined_horizontal_beam, highlight_attribute};

class a_horizontal_scrollbar_with_an_associated_component
  : public a_horizontal_scrollbar
{
 public:
  a_horizontal_scrollbar_with_an_associated_component()
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
    a_horizontal_scrollbar_with_an_associated_component,
    draws_the_lowlight_attribute_by_default)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  ASSERT_EQ(lowlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar_with_an_associated_component,
    draws_the_highlight_attribute_after_the_associated_component_gains_focus)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  scrollbar_->on_redraw.connect(
      [&, this](std::vector<terminalpp::rectangle> const &regions)
      {
        for (auto const &region : regions)
        {
          scrollbar_->draw(surface, region);
        }
      });

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  ASSERT_EQ(highlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar_with_an_associated_component,
    draws_the_lowlight_attribute_after_the_associated_component_loses_focus)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  scrollbar_->on_redraw.connect(
      [&, this](std::vector<terminalpp::rectangle> const &regions)
      {
        for (auto const &region : regions)
        {
          scrollbar_->draw(surface, region);
        }
      });

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(false));
  associated_component_->on_focus_lost();

  ASSERT_EQ(lowlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(lowlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar,
    draws_the_highlight_attribute_when_associated_with_a_focused_component)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  scrollbar_->on_redraw.connect(
      [&, this](std::vector<terminalpp::rectangle> const &regions)
      {
        for (auto const &region : regions)
        {
          scrollbar_->draw(surface, region);
        }
      });

  auto associated_component = make_mock_component();
  ON_CALL(*associated_component, do_has_focus()).WillByDefault(Return(true));

  scrollbar_->set_lowlight_attribute(lowlight_attribute);
  scrollbar_->set_highlight_attribute(highlight_attribute);
  scrollbar_->highlight_on_focus(associated_component);

  ASSERT_EQ(highlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(highlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar_with_an_associated_component,
    draws_the_new_lowlight_attribute_when_it_is_changed)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  scrollbar_->on_redraw.connect(
      [&, this](std::vector<terminalpp::rectangle> const &regions)
      {
        for (auto const &region : regions)
        {
          scrollbar_->draw(surface, region);
        }
      });

  constexpr auto new_lowlight_attribute =
      terminalpp::attribute{terminalpp::high_colour(2, 2, 2)};

  constexpr auto new_lowlight_horizontal_beam = terminalpp::element{
      munin::detail::single_lined_horizontal_beam, new_lowlight_attribute};

  scrollbar_->set_lowlight_attribute(new_lowlight_attribute);

  ASSERT_EQ(new_lowlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(new_lowlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(new_lowlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(new_lowlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar_with_an_associated_component,
    draws_the_new_highlight_attribute_when_it_is_changed)
{
  terminalpp::canvas canvas({8, 1});
  munin::render_surface surface{canvas};

  scrollbar_->set_size({4, 1});
  scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

  scrollbar_->on_redraw.connect(
      [&, this](std::vector<terminalpp::rectangle> const &regions)
      {
        for (auto const &region : regions)
        {
          scrollbar_->draw(surface, region);
        }
      });

  constexpr auto new_highlight_attribute =
      terminalpp::attribute{terminalpp::high_colour(6, 6, 5)};

  constexpr auto new_highlight_horizontal_beam = terminalpp::element{
      munin::detail::single_lined_horizontal_beam, new_highlight_attribute};

  ON_CALL(*associated_component_, do_has_focus()).WillByDefault(Return(true));
  associated_component_->on_focus_set();

  scrollbar_->set_highlight_attribute(new_highlight_attribute);

  ASSERT_EQ(new_highlight_horizontal_beam, canvas[0][0]);
  ASSERT_EQ(new_highlight_horizontal_beam, canvas[1][0]);
  ASSERT_EQ(new_highlight_horizontal_beam, canvas[2][0]);
  ASSERT_EQ(new_highlight_horizontal_beam, canvas[3][0]);
}

TEST_F(
    a_horizontal_scrollbar, with_no_slider_emits_no_scroll_events_when_clicked)
{
  scrollbar_->set_size({5, 1});

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {0, 1}};

  bool scroll_left_clicked = false;
  bool scroll_right_clicked = false;

  scrollbar_->on_scroll_left.connect([&] { scroll_left_clicked = true; });

  scrollbar_->on_scroll_right.connect([&] { scroll_right_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_left_clicked);
  ASSERT_FALSE(scroll_right_clicked);
}

TEST_F(
    a_horizontal_scrollbar,
    with_a_slider_emits_scroll_left_when_clicked_to_the_left_of_the_slider)
{
  scrollbar_->set_size({5, 1});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {0, 0}};

  bool scroll_left_clicked = false;
  bool scroll_right_clicked = false;

  scrollbar_->on_scroll_left.connect([&] { scroll_left_clicked = true; });

  scrollbar_->on_scroll_right.connect([&] { scroll_right_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_TRUE(scroll_left_clicked);
  ASSERT_FALSE(scroll_right_clicked);
}

TEST_F(
    a_horizontal_scrollbar,
    with_a_slider_emits_scroll_right_when_clicked_to_the_right_of_the_slider)
{
  scrollbar_->set_size({5, 1});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {4, 0}};

  bool scroll_left_clicked = false;
  bool scroll_right_clicked = false;

  scrollbar_->on_scroll_left.connect([&] { scroll_left_clicked = true; });

  scrollbar_->on_scroll_right.connect([&] { scroll_right_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_left_clicked);
  ASSERT_TRUE(scroll_right_clicked);
}

TEST_F(
    a_horizontal_scrollbar,
    with_a_slider_emits_nothing_when_clicked_on_the_slider)
{
  scrollbar_->set_size({5, 1});
  scrollbar_->set_slider_position(50, 100);

  auto const mouse_event = terminalpp::mouse::event{
      terminalpp::mouse::event_type::left_button_down, {2, 0}};

  bool scroll_left_clicked = false;
  bool scroll_right_clicked = false;

  scrollbar_->on_scroll_left.connect([&] { scroll_left_clicked = true; });

  scrollbar_->on_scroll_right.connect([&] { scroll_right_clicked = true; });

  scrollbar_->event(mouse_event);

  ASSERT_FALSE(scroll_left_clicked);
  ASSERT_FALSE(scroll_right_clicked);
}