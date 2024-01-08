#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT

TEST(a_new_filled_box, has_a_singular_preferred_size)
{
  munin::filled_box filled_box;
  ASSERT_EQ(terminalpp::extent(1, 1), filled_box.get_preferred_size());
}

TEST(a_new_filled_box, draws_whitespace_on_the_canvas)
{
  munin::filled_box filled_box;
  filled_box.set_size({2, 2});

  terminalpp::canvas canvas({3, 3});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});
  filled_box.draw(surface, {{}, filled_box.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "XXX"_ts,
          "X  "_ts,
          "X  "_ts,
          // clang-format on
      },
      canvas);
}

TEST(a_new_filled_box, refuses_focus)
{
  munin::filled_box filled_box;
  filled_box.set_focus();
  ASSERT_FALSE(filled_box.has_focus());
}
