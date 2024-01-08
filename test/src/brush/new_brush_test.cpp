#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include <munin/brush.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT

TEST(a_new_brush, has_a_singular_preferred_size)
{
  munin::brush brush;
  ASSERT_EQ(terminalpp::extent(1, 1), brush.get_preferred_size());
}

TEST(
    a_new_brush_with_a_single_line_pattern,
    has_a_preferred_size_with_the_width_of_that_line)
{
  static auto const pattern = "abcde"_ts;
  munin::brush brush(pattern);

  static auto const expected_preferred_size = terminalpp::extent{
      static_cast<terminalpp::coordinate_type>(pattern.size()), 1};

  ASSERT_EQ(expected_preferred_size, brush.get_preferred_size());
}

TEST(
    a_new_brush_with_a_multi_line_pattern,
    has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
  static auto const pattern =
      std::vector<terminalpp::string>{"abcde"_ts, "abcdefg"_ts};

  munin::brush brush(pattern);

  static auto const expected_preferred_size = terminalpp::extent{
      static_cast<terminalpp::coordinate_type>(pattern[1].size()), 2};

  ASSERT_EQ(expected_preferred_size, brush.get_preferred_size());
}

TEST(a_new_brush, draws_whitespace_on_the_canvas)
{
  munin::brush brush;
  brush.set_size({2, 2});

  terminalpp::canvas canvas({3, 3});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  brush.draw(surface, {{}, brush.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
      "  X"_ts,
      "  X"_ts,
      "XXX"_ts
          // clang-format on
      },
      canvas);
}

TEST(a_new_brush, refuses_focus)
{
  munin::brush brush;
  brush.set_focus();
  ASSERT_FALSE(brush.has_focus());
}

TEST(a_new_brush_with_a_single_line_pattern, draws_that_pattern_repeatedly)
{
  munin::brush brush("abc"_ts);
  brush.set_size({6, 2});

  terminalpp::canvas canvas({8, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});
  brush.draw(surface, {{}, brush.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
      "XXXXXXXX"_ts,
      "XabcabcX"_ts,
      "XabcabcX"_ts,
      "XXXXXXXX"_ts
          // clang-format on
      },
      canvas);
}

TEST(a_new_brush_with_a_multi_line_pattern, draws_that_pattern_repeatedly)
{
  std::vector<terminalpp::string> pattern = {"ab"_ts, "cd"_ts};

  munin::brush brush(pattern);
  brush.set_size({4, 4});

  terminalpp::canvas canvas({6, 6});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});
  brush.draw(surface, {{}, brush.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
      "XXXXXX"_ts,
      "XababX"_ts,
      "XcdcdX"_ts,
      "XababX"_ts,
      "XcdcdX"_ts,
      "XXXXXX"_ts
          // clang-format on
      },
      canvas);
}

TEST(make_brush_with_no_pattern, makes_a_new_default_brush)
{
  std::shared_ptr<munin::brush> brush = munin::make_brush();
  ASSERT_EQ(terminalpp::extent(1, 1), brush->get_preferred_size());
}

TEST(make_brush_with_a_string_pattern, makes_a_single_line_brush)
{
  std::shared_ptr<munin::brush> brush = munin::make_brush("test"_ts);
  ASSERT_EQ(terminalpp::extent(4, 1), brush->get_preferred_size());
}

TEST(make_brush_with_a_vector_pattern, makes_a_multi_line_brush)
{
  std::vector<terminalpp::string> pattern = {"ab", "cd"};

  std::shared_ptr<munin::brush> brush = munin::make_brush(pattern);
  ASSERT_EQ(terminalpp::extent(2, 2), brush->get_preferred_size());
}
