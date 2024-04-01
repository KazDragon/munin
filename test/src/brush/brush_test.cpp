#include "assert_similar.hpp"
#include "fill_canvas.hpp"

#include <gtest/gtest.h>
#include <munin/brush.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>

using namespace terminalpp::literals;  // NOLINT

TEST(a_brush_with_its_pattern_set_empty, draws_whitespace_on_the_canvas)
{
    munin::brush brush("abc"_ts);

    int called = 0;
    auto on_preferred_size_changed = [&called] { ++called; };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    brush.set_pattern();

    ASSERT_EQ(1, called);

    brush.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});
    fill_canvas(canvas, 'X');

    munin::render_surface cv{canvas};
    brush.draw(cv, {{}, brush.get_size()});

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

TEST(
    a_brush_with_its_pattern_set_to_a_single_line_pattern,
    draws_that_pattern_repeatedly)
{
    munin::brush brush;

    int called = 0;
    auto on_preferred_size_changed = [&called] { ++called; };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    brush.set_pattern("abc"_ts);

    ASSERT_EQ(1, called);

    brush.set_size({6, 2});

    terminalpp::canvas canvas({8, 4});
    fill_canvas(canvas, 'X');

    munin::render_surface cv{canvas};
    cv.offset_by({1, 1});
    brush.draw(cv, {{}, brush.get_size()});

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

TEST(
    a_brush_with_its_pattern_set_to_a_multi_line_pattern,
    draws_that_pattern_repeatedly)
{
    munin::brush brush;

    int called = 0;
    auto on_preferred_size_changed = [&called] { ++called; };
    brush.on_preferred_size_changed.connect(on_preferred_size_changed);

    std::vector<terminalpp::string> const pattern = {"ab"_ts, "cd"_ts};

    brush.set_pattern(pattern);

    ASSERT_EQ(1, called);

    brush.set_size({4, 4});

    terminalpp::canvas canvas({6, 6});
    fill_canvas(canvas, 'X');

    munin::render_surface cv{canvas};
    cv.offset_by({1, 1});
    brush.draw({cv}, {{}, brush.get_size()});

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
