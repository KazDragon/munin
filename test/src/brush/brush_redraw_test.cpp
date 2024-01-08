#include "redraw.hpp"
#include <munin/brush.hpp>
#include <gmock/gmock.h>

using testing::ElementsAreArray;

TEST(a_brush_to_be_redrawn, redraws_entire_brush_when_setting_pattern)
{
  munin::brush brush;
  brush.set_size({6, 3});

  int redraw_called = 0;
  std::vector<terminalpp::rectangle> redraw_regions;
  brush.on_redraw.connect(append_regions_to_container(redraw_regions));

  brush.set_pattern("test");

  ASSERT_THAT(
      redraw_regions,
      ElementsAreArray({terminalpp::rectangle{{0, 0}, {6, 3}}}));
}
