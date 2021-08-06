#include "redraw.hpp"
#include "mock/component.hpp"
#include <munin/scroll_frame.hpp>
#include <munin/detail/unicode_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using testing::Return;

namespace {

class a_new_scroll_frame : public testing::Test
{
protected:
    std::shared_ptr<munin::scroll_frame> scroll_frame_ {
        munin::make_scroll_frame()
    };
};

}

TEST_F(a_new_scroll_frame, is_a_frame)
{
    std::shared_ptr<munin::frame> frame = scroll_frame_;
}

TEST_F(a_new_scroll_frame, draws_a_solid_frame)
{
    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas};

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    scroll_frame_->set_size({4, 4});
    scroll_frame_->draw(surface, {{0, 0}, {4, 4}});

    ASSERT_EQ(munin::detail::single_lined_rounded_top_left_corner,     canvas[0][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][0]);
    ASSERT_EQ(munin::detail::single_lined_rounded_top_right_corner,    canvas[3][0]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'X'},                                canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'},                                canvas[2][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'},                                canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'},                                canvas[2][2]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][2]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][3]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_right_corner, canvas[3][3]);
}

class a_scroll_frame_with_an_associated_component : public a_new_scroll_frame
{
protected :
    a_scroll_frame_with_an_associated_component()
    {
        scroll_frame_->highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus())
            .WillByDefault(Return(false));
    }
    
    std::shared_ptr<mock_component> comp_ = make_mock_component();
};


TEST_F(a_scroll_frame_with_an_associated_component, redraws_when_associated_component_gains_focus)
{
    scroll_frame_->set_size({4, 4});
    
    std::vector<terminalpp::rectangle> redraw_regions;
    scroll_frame_->on_redraw.connect(
        [&redraw_regions](auto const &regions)
        {
            redraw_regions = regions;
        });
    
    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();
    
    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}
    };
    
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
