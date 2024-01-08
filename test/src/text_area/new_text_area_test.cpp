#include "assert_similar.hpp"
#include "redraw.hpp"
#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/string.hpp>

using namespace terminalpp::literals;  // NOLINT
using testing::ValuesIn;
using a_new_text_area = a_text_area;

TEST_F(a_new_text_area, has_zero_cursor_position)
{
  ASSERT_EQ(terminalpp::point(0, 0), text_area_.get_cursor_position());
}

TEST_F(a_new_text_area, has_cursor_enabled)
{
  ASSERT_TRUE(text_area_.get_cursor_state());
}

TEST_F(a_new_text_area, has_caret_position_zero)
{
  ASSERT_EQ(0, text_area_.get_caret_position());
}

TEST_F(a_new_text_area, has_length_zero)
{
  ASSERT_EQ(0, text_area_.get_length());
}

TEST_F(a_new_text_area, has_preferred_size_of_0x1)
{
  // The preferred size always has width of its current size, but it also
  // wants a line to put the cursor in.
  ASSERT_EQ(terminalpp::extent(0, 1), text_area_.get_preferred_size());
}

TEST_F(a_new_text_area, draws_only_spaces)
{
  text_area_.set_size({2, 2});
  fill_canvas({3, 3});

  munin::render_surface surface{canvas_};
  text_area_.draw(surface, {{}, text_area_.get_size()});

  assert_similar_canvas_block(
      {
          // clang-format off
          "  X"_ts,
          "  X"_ts,
          "XXX"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(
    a_new_text_area,
    announces_cursor_position_change_when_inserting_text_at_the_caret)
{
  text_area_.set_size({2, 2});

  bool cursor_position_changed = false;
  text_area_.on_cursor_position_changed.connect(
      [&cursor_position_changed]() { cursor_position_changed = true; });

  text_area_.insert_text("a"_ts);

  ASSERT_TRUE(cursor_position_changed);
  ASSERT_EQ(terminalpp::point(1, 0), text_area_.get_cursor_position());
  ASSERT_EQ(1, text_area_.get_caret_position());
  ASSERT_EQ(terminalpp::extent(2, 1), text_area_.get_preferred_size());
  ASSERT_EQ(munin::text_area::text_index{1}, text_area_.get_length());
}

TEST_F(
    a_new_text_area,
    requests_a_redraw_and_draws_inserted_text_when_text_is_inserted)
{
  text_area_.set_size({2, 2});

  std::vector<terminalpp::rectangle> redraw_regions;

  text_area_.on_redraw.connect(append_regions_to_container(redraw_regions));

  text_area_.insert_text("b"_ts);

  ASSERT_FALSE(redraw_regions.empty());

  fill_canvas({3, 3});
  munin::render_surface surface{canvas_};

  for (auto const &region : redraw_regions)
  {
    text_area_.draw(surface, region);
  }

  assert_similar_canvas_block(
      {
          // clang-format off
          "b X"_ts,
          "  X"_ts,
          "XXX"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(a_new_text_area, flows_long_text_into_the_next_line)
{
  text_area_.set_size({2, 2});

  std::vector<terminalpp::rectangle> redraw_regions;

  text_area_.on_redraw.connect(append_regions_to_container(redraw_regions));

  text_area_.insert_text("cde"_ts);

  ASSERT_FALSE(redraw_regions.empty());

  fill_canvas({3, 3});
  munin::render_surface surface{canvas_};

  for (auto const &region : redraw_regions)
  {
    text_area_.draw(surface, region);
  }

  assert_similar_canvas_block(
      {
          // clang-format off
          "cdX"_ts,
          "e X"_ts,
          "XXX"_ts,
          // clang-format on
      },
      canvas_);
}

TEST_F(a_new_text_area, flows_newlines_when_drawing_text)
{
  text_area_.set_size({2, 2});

  std::vector<terminalpp::rectangle> redraw_regions;

  text_area_.on_redraw.connect(append_regions_to_container(redraw_regions));

  text_area_.insert_text("c\nde"_ts);

  ASSERT_FALSE(redraw_regions.empty());

  fill_canvas({3, 3});
  munin::render_surface surface{canvas_};

  for (auto const &region : redraw_regions)
  {
    text_area_.draw(surface, region);
  }

  assert_similar_canvas_block(
      {
          // clang-format off
          "c X"_ts,
          "deX"_ts,
          "XXX"_ts,
          // clang-format on
      },
      canvas_);
}

using text_area_layout_data = std::tuple<
    terminalpp::extent,            // actual size
    terminalpp::string,            // actual text content
    terminalpp::extent,            // expected preferred size
    munin::text_area::text_index,  // expected caret position
    terminalpp::point              // expected cursor position
    >;

class unjustified_text_areas
  : public testing::TestWithParam<text_area_layout_data>
{
 protected:
  unjustified_text_areas()
  {
    auto const &param = GetParam();
    auto const &size = std::get<0>(param);
    auto const &text = std::get<1>(param);
    expected_preferred_size_ = std::get<2>(param);
    expected_caret_position_ = std::get<3>(param);
    expected_cursor_position_ = std::get<4>(param);

    text_area_.set_size(size);
    text_area_.insert_text(text);
  }

  munin::text_area text_area_;
  terminalpp::extent expected_preferred_size_;
  munin::text_area::text_index expected_caret_position_;
  terminalpp::point expected_cursor_position_;
};

TEST_P(unjustified_text_areas, have_a_preferred_size_related_to_text_content)
{
  ASSERT_EQ(expected_preferred_size_, text_area_.get_preferred_size());
};

TEST_P(unjustified_text_areas, have_a_specific_caret_position_after_insertion)
{
  ASSERT_EQ(expected_caret_position_, text_area_.get_caret_position());
}

TEST_P(unjustified_text_areas, have_a_specific_cursor_position_after_insertion)
{
  ASSERT_EQ(expected_cursor_position_, text_area_.get_cursor_position());
}

INSTANTIATE_TEST_SUITE_P(
    unjustified_text_areas_behave,
    unjustified_text_areas,
    ValuesIn({
        // Default position (nothing was inserted)
        text_area_layout_data{{3, 4}, ""_ts, {3, 1}, 0, {0, 0}},

        // Insertions that do not require flow (manual newlines only)
        text_area_layout_data{{3, 4}, "a"_ts, {3, 1}, 1, {1, 0}},
        text_area_layout_data{{3, 4}, "ab"_ts, {3, 1}, 2, {2, 0}},
        text_area_layout_data{{3, 4}, "a\nb"_ts, {3, 2}, 3, {1, 1}},
        text_area_layout_data{{3, 4}, "ab\n"_ts, {3, 2}, 3, {0, 1}},
        text_area_layout_data{{3, 4}, "ab\nc"_ts, {3, 2}, 4, {1, 1}},

        // Insertions that require flow (had an automatic split)
        text_area_layout_data{{3, 4}, "abc"_ts, {3, 2}, 3, {0, 1}},
        text_area_layout_data{{3, 4}, "abcd"_ts, {3, 2}, 4, {1, 1}},
        text_area_layout_data{{3, 4}, "abcde"_ts, {3, 2}, 5, {2, 1}},
        text_area_layout_data{{3, 4}, "abcdef"_ts, {3, 3}, 6, {0, 2}},
        text_area_layout_data{{3, 4}, "abcdefg"_ts, {3, 3}, 7, {1, 2}},

        // Insertions that have an explicit newline on the boundary
        text_area_layout_data{{3, 4}, "abc\n", {3, 2}, 4, {0, 1}},
    }));

TEST_F(
    a_new_text_area,
    does_not_move_the_caret_when_inserting_at_a_specified_index_but_still_inserts_the_text)
{
  text_area_.set_size({2, 2});

  bool cursor_position_changed = false;
  text_area_.on_cursor_position_changed.connect(
      [&cursor_position_changed]() { cursor_position_changed = true; });

  bool preferred_size_changed = false;
  text_area_.on_preferred_size_changed.connect(
      [&preferred_size_changed]() { preferred_size_changed = true; });

  std::vector<terminalpp::rectangle> redraw_regions;

  text_area_.on_redraw.connect(append_regions_to_container(redraw_regions));

  text_area_.insert_text("a"_ts, 0);

  ASSERT_TRUE(preferred_size_changed);
  ASSERT_EQ(terminalpp::extent(2, 1), text_area_.get_preferred_size());

  ASSERT_FALSE(cursor_position_changed);
  ASSERT_EQ(terminalpp::point(0, 0), text_area_.get_cursor_position());

  ASSERT_EQ(0, text_area_.get_caret_position());

  ASSERT_FALSE(redraw_regions.empty());

  fill_canvas({3, 3});
  munin::render_surface surface{canvas_};

  for (auto const &region : redraw_regions)
  {
    text_area_.draw(surface, region);
  }

  assert_similar_canvas_block(
      {
          // clang-format off
          "a X"_ts,
          "  X"_ts,
          "XXX"_ts,
          // clang-format on
      },
      canvas_);
}
