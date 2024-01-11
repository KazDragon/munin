#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "mock/component.hpp"
#include "redraw.hpp"
#include <munin/detail/border_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <munin/scroll_frame.hpp>
#include <gtest/gtest.h>

using testing::Return;

namespace {

class a_new_scroll_frame : public testing::Test
{
 protected:
  std::shared_ptr<munin::scroll_frame> scroll_frame_{
      munin::make_scroll_frame()};
};

}  // namespace

TEST_F(a_new_scroll_frame, is_a_frame)
{
  std::shared_ptr<munin::frame> frame = scroll_frame_;
}

TEST_F(a_new_scroll_frame, draws_a_solid_frame)
{
  terminalpp::canvas canvas({4, 4});
  fill_canvas(canvas, 'X');

  scroll_frame_->set_size({4, 4});

  munin::render_surface surface{canvas};
  scroll_frame_->draw(surface, {{0, 0}, {4, 4}});

  assert_similar_canvas_block(
      {
          // clang-format off
          { 
            munin::detail::border::unicode::top_left_corner,
            munin::detail::border::unicode::horizontal_beam,
            munin::detail::border::unicode::horizontal_beam,
            munin::detail::border::unicode::top_right_corner,
          },
          {
            munin::detail::border::unicode::vertical_beam,
            'X',
            'X',
            munin::detail::border::unicode::vertical_beam,
          },
          {
            munin::detail::border::unicode::vertical_beam,
            'X',
            'X',
            munin::detail::border::unicode::vertical_beam,
          },
          { 
            munin::detail::border::unicode::bottom_left_corner,
            munin::detail::border::unicode::horizontal_beam,
            munin::detail::border::unicode::horizontal_beam,
            munin::detail::border::unicode::bottom_right_corner,
          },
          // clang-format on
      },
      canvas);
}

class a_scroll_frame_with_an_associated_component : public a_new_scroll_frame
{
 protected:
  a_scroll_frame_with_an_associated_component()
  {
    scroll_frame_->highlight_on_focus(comp_);
    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
  }

  std::shared_ptr<mock_component> comp_ = make_mock_component();
};

TEST_F(
    a_scroll_frame_with_an_associated_component,
    redraws_when_associated_component_gains_focus)
{
  scroll_frame_->set_size({4, 4});

  std::vector<terminalpp::rectangle> redraw_regions;
  scroll_frame_->on_redraw.connect(append_regions_to_container(redraw_regions));

  ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
  comp_->on_focus_set();

  std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}};

  assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
