#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include <munin/image.hpp>
#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT

TEST(an_image_with_its_content_set_empty, draws_fill_on_the_canvas)
{
  munin::image image("test"_ts, ' ');

  image.set_content();

  image.set_size({6, 3});
  terminalpp::canvas canvas({6, 3});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  image.draw(surface, {{}, image.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "      "_ts,
          "      "_ts,
          "      "_ts,
          // clang-format on
      },
      canvas);
}

TEST(an_image_with_its_content_set_to_single_line, draws_line_on_the_canvas)
{
  munin::image image(' ');
  image.set_size({6, 3});

  image.set_content("test"_ts);

  terminalpp::canvas canvas({6, 3});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  image.draw(surface, {{}, image.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "      "_ts,
          " test "_ts,
          "      "_ts,
          // clang-format on
      },
      canvas);
}

TEST(an_image_with_its_content_set_to_multi_line, draws_lines_on_the_canvas)
{
  munin::image image(' ');
  image.set_size({6, 4});

  std::vector<terminalpp::string> const content = {"test"_ts, "abcd"_ts};

  image.set_content(content);

  terminalpp::canvas canvas({6, 4});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  image.draw(surface, {{}, image.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "      "_ts,
          " test "_ts,
          " abcd "_ts,
          "      "_ts,
          // clang-format on
      },
      canvas);
}

TEST(an_image, sets_its_content_empty_when_set_to_an_empty_string)
{
  munin::image image("test");
  image.set_size({4, 1});

  image.set_content("");

  ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());

  terminalpp::canvas canvas({4, 1});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  image.draw(surface, {{}, image.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "      "_ts,
          // clang-format on
      },
      canvas);
}

TEST(an_image, can_have_its_fill_set)
{
  munin::image image("test");
  image.set_size({6, 3});

  image.set_fill('!');

  terminalpp::canvas canvas({6, 3});
  fill_canvas(canvas, 'X');

  munin::render_surface surface{canvas};
  image.draw(surface, {{}, image.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "!!!!!!"_ts,
          "!test!"_ts,
          "!!!!!!"_ts,
          // clang-format on
      },
      canvas);
}
