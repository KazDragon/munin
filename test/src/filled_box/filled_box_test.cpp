#include "assert_similar.hpp"
#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;  // NOLINT

TEST(make_fill, makes_a_new_filled_box)
{
  std::shared_ptr<munin::filled_box> filled_box = munin::make_fill('F');
}

TEST(a_filled_box, can_have_its_preferred_size_set)
{
  int called = 0;
  auto on_preferred_size_changed = [&called] { ++called; };

  munin::filled_box filled_box;
  filled_box.on_preferred_size_changed.connect(on_preferred_size_changed);

  filled_box.set_preferred_size({2, 2});

  ASSERT_EQ(terminalpp::extent(2, 2), filled_box.get_preferred_size());
  ASSERT_EQ(1, called);
}

TEST(a_filled_box, draws_its_fill)
{
  munin::filled_box filled_box('Y');
  filled_box.set_size({1, 1});

  terminalpp::canvas canvas({2, 2});
  munin::render_surface surface{canvas};

  filled_box.draw(surface, {{}, {1, 1}});

  assert_similar_canvas_block(
      {
          // clang-format off
          "Y "_ts,
          "  "_ts,
          // clang-format on
      },
      canvas);
}

TEST(a_filled_box, draws_only_within_given_region)
{
  munin::filled_box filled_box('Y');
  filled_box.set_size({2, 2});

  terminalpp::canvas canvas({3, 4});
  munin::render_surface surface{canvas};
  surface.offset_by({1, 1});

  filled_box.draw(surface, {{}, {1, 2}});

  assert_similar_canvas_block(
      {
          // clang-format off
          "   "_ts,
          " Y "_ts,
          " Y "_ts,
          "   "_ts,
          // clang-format on
      },
      canvas);
}

TEST(a_filled_box, reports_attributes_as_json)
{
  munin::filled_box box;
  munin::component &component = box;

  terminalpp::point position{37, 20};
  terminalpp::extent size{4, 5};
  terminalpp::extent preferred_size{7, 8};

  box.set_preferred_size(preferred_size);
  component.set_position(position);
  component.set_size(size);

  nlohmann::json json = component.to_json();
  ASSERT_EQ("filled_box", json["type"]);
  ASSERT_EQ(position.x_, json["position"]["x"]);
  ASSERT_EQ(position.y_, json["position"]["y"]);
  ASSERT_EQ(size.width_, json["size"]["width"]);
  ASSERT_EQ(size.height_, json["size"]["height"]);
  ASSERT_EQ(preferred_size.width_, json["preferred_size"]["width"]);
  ASSERT_EQ(preferred_size.height_, json["preferred_size"]["height"]);
  ASSERT_EQ(false, json["has_focus"]);
  ASSERT_EQ(false, json["cursor_state"]);
  ASSERT_EQ(0, json["cursor_position"]["x"]);
  ASSERT_EQ(0, json["cursor_position"]["y"]);
}
