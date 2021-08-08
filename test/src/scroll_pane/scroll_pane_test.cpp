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

        ON_CALL(*inner_component_, do_get_preferred_size())
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
    terminalpp::point cursor_position;
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
    ASSERT_EQ(munin::detail::single_lined_cross,                       canvas[3][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'1'},                                canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'2'},                                canvas[2][2]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][2]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(munin::detail::single_lined_cross,                       canvas[1][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][3]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_right_corner, canvas[3][3]);
}

TEST_F(a_new_scroll_pane, tracks_the_cursor_in_the_inner_component)
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


    ON_CALL(*inner_component_, do_get_cursor_position())
        .WillByDefault(Return(terminalpp::point(9, 9)));
    inner_component_->on_cursor_position_changed();

    scroll_pane->draw(surface, {{0, 0}, {4, 4}});

    ASSERT_EQ(munin::detail::single_lined_rounded_top_left_corner,     canvas[0][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[2][0]);
    ASSERT_EQ(munin::detail::single_lined_rounded_top_right_corner,    canvas[3][0]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'6'},                                canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'7'},                                canvas[2][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[3][1]);
    ASSERT_EQ(munin::detail::single_lined_vertical_beam,               canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'7'},                                canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'8'},                                canvas[2][2]);
    ASSERT_EQ(munin::detail::single_lined_cross,                       canvas[3][2]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam,             canvas[1][3]);
    ASSERT_EQ(munin::detail::single_lined_cross,                       canvas[2][3]);
    ASSERT_EQ(munin::detail::single_lined_rounded_bottom_right_corner, canvas[3][3]);
}

namespace {

constexpr auto highlight_attribute = terminalpp::attribute{
    terminalpp::high_colour(5, 4, 1)
};

constexpr auto highlighted_single_lined_rounded_top_left_corner = terminalpp::element{
    munin::detail::single_lined_rounded_top_left_corner,
    highlight_attribute
};

constexpr auto highlighted_single_lined_horizontal_beam = terminalpp::element{
    munin::detail::single_lined_horizontal_beam,
    highlight_attribute
};

constexpr auto highlighted_single_lined_cross = terminalpp::element{
    munin::detail::single_lined_cross,
    highlight_attribute
};

constexpr auto highlighted_single_lined_rounded_top_right_corner = terminalpp::element{
    munin::detail::single_lined_rounded_top_right_corner,
    highlight_attribute
};

constexpr auto highlighted_single_lined_vertical_beam = terminalpp::element{
    munin::detail::single_lined_vertical_beam,
    highlight_attribute
};

constexpr auto highlighted_single_lined_rounded_bottom_left_corner = terminalpp::element{
    munin::detail::single_lined_rounded_bottom_left_corner,
    highlight_attribute
};

constexpr auto highlighted_single_lined_rounded_bottom_right_corner = terminalpp::element{
    munin::detail::single_lined_rounded_bottom_right_corner,
    highlight_attribute
};

}

TEST_F(a_new_scroll_pane, highlights_the_whole_frame_when_the_underlying_component_receives_focus)
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
    scroll_pane->set_highlight_attribute(highlight_attribute);
    
    ON_CALL(*inner_component_, do_has_focus())
        .WillByDefault(Return(true));
    inner_component_->on_focus_set();

    scroll_pane->draw(surface, {{0, 0}, {4, 4}});

    EXPECT_EQ(highlighted_single_lined_rounded_top_left_corner,     canvas[0][0]);
    EXPECT_EQ(highlighted_single_lined_horizontal_beam,             canvas[1][0]);
    EXPECT_EQ(highlighted_single_lined_horizontal_beam,             canvas[2][0]);
    EXPECT_EQ(highlighted_single_lined_rounded_top_right_corner,    canvas[3][0]);
    EXPECT_EQ(highlighted_single_lined_vertical_beam,               canvas[0][1]);
    EXPECT_EQ(terminalpp::element{'0'},                             canvas[1][1]);
    EXPECT_EQ(terminalpp::element{'1'},                             canvas[2][1]);
    EXPECT_EQ(highlighted_single_lined_cross,                       canvas[3][1]);
    EXPECT_EQ(highlighted_single_lined_vertical_beam,               canvas[0][2]);
    EXPECT_EQ(terminalpp::element{'1'},                             canvas[1][2]);
    EXPECT_EQ(terminalpp::element{'2'},                             canvas[2][2]);
    EXPECT_EQ(highlighted_single_lined_vertical_beam,               canvas[3][2]);
    EXPECT_EQ(highlighted_single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    EXPECT_EQ(highlighted_single_lined_cross,                       canvas[1][3]);
    EXPECT_EQ(highlighted_single_lined_horizontal_beam,             canvas[2][3]);
    EXPECT_EQ(highlighted_single_lined_rounded_bottom_right_corner, canvas[3][3]);
}

namespace {

constexpr auto lowlight_attribute = terminalpp::attribute{
    terminalpp::high_colour(2, 1, 2)
};

constexpr auto lowlighted_single_lined_rounded_top_left_corner = terminalpp::element{
    munin::detail::single_lined_rounded_top_left_corner,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_horizontal_beam = terminalpp::element{
    munin::detail::single_lined_horizontal_beam,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_cross = terminalpp::element{
    munin::detail::single_lined_cross,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_rounded_top_right_corner = terminalpp::element{
    munin::detail::single_lined_rounded_top_right_corner,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_vertical_beam = terminalpp::element{
    munin::detail::single_lined_vertical_beam,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_rounded_bottom_left_corner = terminalpp::element{
    munin::detail::single_lined_rounded_bottom_left_corner,
    lowlight_attribute
};

constexpr auto lowlighted_single_lined_rounded_bottom_right_corner = terminalpp::element{
    munin::detail::single_lined_rounded_bottom_right_corner,
    lowlight_attribute
};

}

TEST_F(a_new_scroll_pane, lowlights_the_whole_frame_when_the_underlying_component_loses_focus)
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
    scroll_pane->set_highlight_attribute(highlight_attribute);
    scroll_pane->set_lowlight_attribute(lowlight_attribute);
    
    ON_CALL(*inner_component_, do_has_focus())
        .WillByDefault(Return(true));
    inner_component_->on_focus_set();

    ON_CALL(*inner_component_, do_has_focus())
        .WillByDefault(Return(false));
    inner_component_->on_focus_lost();

    scroll_pane->draw(surface, {{0, 0}, {4, 4}});

    EXPECT_EQ(lowlighted_single_lined_rounded_top_left_corner,     canvas[0][0]);
    EXPECT_EQ(lowlighted_single_lined_horizontal_beam,             canvas[1][0]);
    EXPECT_EQ(lowlighted_single_lined_horizontal_beam,             canvas[2][0]);
    EXPECT_EQ(lowlighted_single_lined_rounded_top_right_corner,    canvas[3][0]);
    EXPECT_EQ(lowlighted_single_lined_vertical_beam,               canvas[0][1]);
    EXPECT_EQ(terminalpp::element{'0'},                            canvas[1][1]);
    EXPECT_EQ(terminalpp::element{'1'},                            canvas[2][1]);
    EXPECT_EQ(lowlighted_single_lined_cross,                       canvas[3][1]);
    EXPECT_EQ(lowlighted_single_lined_vertical_beam,               canvas[0][2]);
    EXPECT_EQ(terminalpp::element{'1'},                            canvas[1][2]);
    EXPECT_EQ(terminalpp::element{'2'},                            canvas[2][2]);
    EXPECT_EQ(lowlighted_single_lined_vertical_beam,               canvas[3][2]);
    EXPECT_EQ(lowlighted_single_lined_rounded_bottom_left_corner,  canvas[0][3]);
    EXPECT_EQ(lowlighted_single_lined_cross,                       canvas[1][3]);
    EXPECT_EQ(lowlighted_single_lined_horizontal_beam,             canvas[2][3]);
    EXPECT_EQ(lowlighted_single_lined_rounded_bottom_right_corner, canvas[3][3]);
}

TEST(a_scroll_pane, with_a_resize_strategy_will_resize_the_tracked_component_accordingly)
{
    auto mock_component = make_mock_component();

    auto component_size = terminalpp::extent{};
    ON_CALL(*mock_component, do_set_size(_))
        .WillByDefault(SaveArg<0>(&component_size));
    ON_CALL(*mock_component, do_get_size())
        .WillByDefault(ReturnPointee(&component_size));

    auto const preferred_size = terminalpp::extent{10, 10};
    ON_CALL(*mock_component, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));

    auto scroll_pane = munin::make_scroll_pane(
        mock_component,
        munin::make_horizontal_viewport_resize_strategy());

    scroll_pane->set_size({7, 7});

    // The scroll pane reserves two rows for its border.
    auto const expected_size = terminalpp::extent{10, 5};
    ASSERT_EQ(expected_size, component_size);
}
