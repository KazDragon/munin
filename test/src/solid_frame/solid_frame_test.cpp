#include "solid_frame_test.hpp"
#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "mock/component.hpp"
#include "redraw.hpp"
#include <terminalpp/attribute.hpp>
#include <terminalpp/string.hpp>

using namespace terminalpp::literals;  // NOLINT
using testing::Return;

namespace {

constexpr auto highlight_attribute = terminalpp::attribute(
    terminalpp::graphics::colour::cyan,
    terminalpp::colour(),
    terminalpp::graphics::intensity::bold);

void assert_ansi_frame_with_attribute(
    terminalpp::canvas &canvas,
    terminalpp::attribute const &attr = terminalpp::attribute{})
{
  static auto const top_left_corner = R"(+)"_ete.glyph_;
  static auto const top_right_corner = R"(+)"_ete.glyph_;
  static auto const bottom_left_corner = R"(+)"_ete.glyph_;
  static auto const bottom_right_corner = R"(+)"_ete.glyph_;
  static auto const horizontal_beam = R"(-)"_ete.glyph_;
  static auto const vertical_beam = R"(|)"_ete.glyph_;

  assert_similar_canvas_block(
      {
          // clang-format off
          {
            { top_left_corner, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { top_right_corner, attr },
          },
          {
            { vertical_beam, attr },
            'X',
            'X',
            { vertical_beam, attr },
          },
          {
            { vertical_beam, attr },
            'X',
            'X',
            { vertical_beam, attr },
          },
          {
            { bottom_left_corner, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { bottom_right_corner, attr },
          },
          // clang-format on
      },
      canvas);
}

void assert_unicode_frame_with_attribute(
    terminalpp::canvas &canvas,
    terminalpp::attribute const &attr = terminalpp::attribute{})
{
  static auto const top_left_corner = R"(\U256D)"_ete.glyph_;
  static auto const top_right_corner = R"(\U256E)"_ete.glyph_;
  static auto const bottom_left_corner = R"(\U2570)"_ete.glyph_;
  static auto const bottom_right_corner = R"(\U256F)"_ete.glyph_;
  static auto const horizontal_beam = R"(\U2500)"_ete.glyph_;
  static auto const vertical_beam = R"(\U2502)"_ete.glyph_;

  assert_similar_canvas_block(
      {
          // clang-format off
          {
            { top_left_corner, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { top_right_corner, attr },
          },
          {
            { vertical_beam, attr },
            'X',
            'X',
            { vertical_beam, attr },
          },
          {
            { vertical_beam, attr },
            'X',
            'X',
            { vertical_beam, attr },
          },
          {
            { bottom_left_corner, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { bottom_right_corner, attr },
          },
          // clang-format on
      },
      canvas);
}

}  // namespace

TEST_F(a_solid_frame, is_a_component)
{
  std::shared_ptr<munin::component> comp = munin::make_solid_frame();
}

TEST_F(a_solid_frame_with_no_unicode_support, draws_a_border)
{
  frame_.set_size({4, 4});

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas);
}

TEST_F(
    a_solid_frame_with_unicode_support, draws_a_border_with_box_drawing_glyphs)
{
  frame_.set_size({4, 4});

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_unicode_frame_with_attribute(canvas);
}

TEST_F(a_solid_frame, can_be_displayed_with_a_custom_lowlight)
{
  static constexpr auto lowlight_attribute = terminalpp::attribute(
      terminalpp::graphics::colour::green,
      terminalpp::colour(),
      terminalpp::graphics::intensity::bold);

  frame_.set_size({4, 4});
  frame_.set_lowlight_attribute(lowlight_attribute);

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas, lowlight_attribute);
}

class a_solid_frame_with_an_associated_unfocussed_component
  : public a_solid_frame_with_no_unicode_support
{
 protected:
  a_solid_frame_with_an_associated_unfocussed_component()
  {
    frame_.highlight_on_focus(comp_);
    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
  }

  std::shared_ptr<mock_component> comp_ = make_mock_component();
};

class a_solid_frame_with_an_associated_focussed_component
  : public a_solid_frame_with_no_unicode_support
{
 protected:
  a_solid_frame_with_an_associated_focussed_component()
  {
    frame_.highlight_on_focus(comp_);
    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();
  }

  std::shared_ptr<mock_component> comp_ = make_mock_component();
};

TEST_F(
    a_solid_frame_with_an_associated_unfocussed_component,
    draws_a_lowlighted_border)
{
  frame_.set_size({4, 4});

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas);
}

TEST_F(
    a_solid_frame_with_an_associated_unfocussed_component,
    when_focussed_draws_a_highlighted_border)
{
  frame_.set_size({4, 4});

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
  comp_->on_focus_set();

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas, highlight_attribute);
}

TEST_F(
    a_solid_frame_with_an_associated_unfocussed_component,
    redraws_when_associated_component_gains_focus)
{
  frame_.set_size({4, 4});

  int redraw_count = 0;
  std::vector<terminalpp::rectangle> redraw_regions;
  frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
  comp_->on_focus_set();

  std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}};

  assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(
    a_solid_frame_with_an_associated_unfocussed_component,
    redraws_a_reduced_amount_when_associated_component_gains_focus_when_small)
{
  frame_.set_size({1, 1});

  std::vector<terminalpp::rectangle> redraw_regions;
  frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
  comp_->on_focus_set();

  std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {1, 1}},
  };

  assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);

  for (auto region : redraw_regions)
  {
    ASSERT_GE(region.origin_.x_, 0);
    ASSERT_GE(region.origin_.y_, 0);
    ASSERT_GE(region.size_.width_, 0);
    ASSERT_GE(region.size_.height_, 0);
  }
}

TEST_F(
    a_solid_frame_with_an_associated_focussed_component,
    when_unfocussed_draws_a_lowlit_border)
{
  frame_.set_size({4, 4});

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
  comp_->on_focus_lost();

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas);
}

TEST_F(
    a_solid_frame_with_an_associated_focussed_component,
    can_have_a_custom_highlight)
{
  static constexpr auto custom_highlight = terminalpp::attribute{
      terminalpp::graphics::colour::green,
      terminalpp::graphics::colour::magenta};

  frame_.set_highlight_attribute(custom_highlight);
  frame_.set_size({4, 4});

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
  comp_->on_focus_set();

  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas, surface_capabilities_};
  frame_.draw(surface, {{}, {4, 4}});

  assert_ansi_frame_with_attribute(canvas, custom_highlight);
}

TEST_F(
    a_solid_frame_with_an_associated_focussed_component,
    redraws_when_associated_component_loses_focus)
{
  frame_.set_size({4, 4});

  std::vector<terminalpp::rectangle> redraw_regions;
  frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
  comp_->on_focus_lost();

  std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}};

  assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
