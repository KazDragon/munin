#include <munin/horizontal_scrollbar.hpp>
#include <munin/detail/unicode_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

namespace {

class a_horizontal_scrollbar : public testing::Test
{
protected:
    std::shared_ptr<munin::horizontal_scrollbar> scrollbar_ {
        munin::make_horizontal_scrollbar()
    };
};

using a_new_horizontal_scrollbar = a_horizontal_scrollbar;

}

TEST_F(a_new_horizontal_scrollbar, draws_nothing)
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

    scrollbar_->draw(surface, {{0, 0}, {0, 0}});

    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][3]);
}

TEST_F(a_horizontal_scrollbar, with_size_but_no_slider_draws_a_frame_border)
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

    scrollbar_->set_size({4, 1});
    scrollbar_->draw(surface, {{}, scrollbar_->get_size()});

    ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[0][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[1][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[2][0]);
    ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][3]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[3][3]);
}