#include <munin/scroll_pane.hpp>
#include <munin/detail/unicode_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include "mock/component.hpp"
#include <gtest/gtest.h>

using testing::_;
using testing::Return;
using testing::ReturnPointee;
using testing::SaveArg;
using namespace terminalpp::literals;

namespace {

class a_new_scroll_pane : public testing::Test
{
public:
    a_new_scroll_pane()
    {
        // Set up our mock component as a 10x10 grid of digits
        ON_CALL(*inner_component_, do_draw(_, _))
            .WillByDefault(
                [this](munin::render_surface &surface, terminalpp::rectangle const &region)
                {
                    terminalpp::for_each_in_region(
                        surface,
                        region,
                        [](terminalpp::element &elem,
                           terminalpp::coordinate_type column,
                           terminalpp::coordinate_type row)
                        {
                            elem = '0' + ((column + row) % 10);
                        });
                });

        ON_CALL(*inner_component_, do_get_preferred_size)
            .WillByDefault(Return(terminalpp::extent{10, 10}));

        ON_CALL(*inner_component_, do_set_position(_))
            .WillByDefault(SaveArg<0>(&mock_position));

        ON_CALL(*inner_component_, do_get_position())
            .WillByDefault(ReturnPointee(&mock_position));

        ON_CALL(*inner_component_, do_set_size(_))
            .WillByDefault(SaveArg<0>(&mock_size));

        ON_CALL(*inner_component_, do_get_size())
            .WillByDefault(ReturnPointee(&mock_size));
    }

protected:
    terminalpp::point mock_position;
    terminalpp::extent mock_size;
    std::shared_ptr<mock_component> inner_component_ {
        make_mock_component()
    };

    std::shared_ptr<munin::scroll_pane> scroll_pane {
        munin::make_scroll_pane(inner_component_)
    };
};

}

TEST_F(a_new_scroll_pane, draws_the_component_within_its_frame)
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

    scroll_pane->set_size({4, 4});
    scroll_pane->draw(surface, {{0, 0}, {4, 4}});

    ASSERT_EQ(munin::detail::single_lined_rounded_top_left_corner,     canvas[0][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][0]);
    ASSERT_EQ(munin::detail::single_lined_rounded_top_right_corner,    canvas[3][0]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'0'},                                canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'1'},                                canvas[2][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'1'},                                canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'2'},                                canvas[2][2]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][2]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][3]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_right_corner, canvas[3][3]);
}
