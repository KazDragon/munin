#include "assert_similar.hpp"
#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT

TEST(a_filled_box, can_retrieve_its_fill_from_a_function)
{
  terminalpp::element fill = 'x';

  auto fill_function = [&fill](munin::render_surface &) { return fill; };

  auto box = munin::make_fill(fill_function);
  box->set_size({2, 2});

  terminalpp::canvas canvas({2, 2});
  munin::render_surface surface(canvas);

  box->draw(surface, terminalpp::rectangle{{}, {2, 2}});

  assert_similar_canvas_block(
      {
          // clang-format off
          "xx"_ts,
          "xx"_ts,
          // clang-format on
      },
      canvas);

  fill = 'y';

  box->draw(surface, terminalpp::rectangle{{}, {2, 2}});

  assert_similar_canvas_block(
      {
          // clang-format off
          "yy"_ts,
          "yy"_ts,
          // clang-format on
      },
      canvas);
}