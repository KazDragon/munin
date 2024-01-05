#include <munin/brush.hpp>
#include <gtest/gtest.h>

TEST(a_brush_to_be_redrawn, redraws_entire_brush_when_setting_pattern)
{
  munin::brush brush;
  brush.set_size({6, 3});

  int redraw_called = 0;
  std::vector<terminalpp::rectangle> redraw_regions;
  brush.on_redraw.connect(
      [&redraw_called, &redraw_regions](auto const &regions)
      {
        ++redraw_called;
        redraw_regions = regions;
      });

  brush.set_pattern("test");

  ASSERT_EQ(1, redraw_called);
  ASSERT_EQ(1u, redraw_regions.size());
  ASSERT_EQ(terminalpp::rectangle({0, 0}, {6, 3}), redraw_regions[0]);
}
