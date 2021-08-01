#include <munin/horizontal_scrollbar.hpp>
#include <munin/detail/unicode_glyphs.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using testing::ValuesIn;

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

TEST_F(a_new_horizontal_scrollbar, has_a_0x1_preferred_size)
{
    ASSERT_EQ(terminalpp::extent(0, 1), scrollbar_->get_preferred_size());
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

TEST_F(a_horizontal_scrollbar, has_a_preferred_size_relative_to_its_width)
{
    scrollbar_->set_size({4, 4});
    ASSERT_EQ(terminalpp::extent(4, 1), scrollbar_->get_preferred_size());

    scrollbar_->set_size({7, 3});
    ASSERT_EQ(terminalpp::extent(7, 1), scrollbar_->get_preferred_size());
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

namespace {

using slider_test_data = std::tuple<
    terminalpp::coordinate_type, // x_position
    terminalpp::coordinate_type, // width
    terminalpp::coordinate_type  // expected x co-ordinate of slider
>;

class horizontal_scroll_bar_slider_position
  : public testing::TestWithParam<slider_test_data>
{
protected:
    horizontal_scroll_bar_slider_position()
    {
        scrollbar_->set_size(canvas_.size());
        
        terminalpp::for_each_in_region(
            canvas_,
            {{}, canvas_.size()},
            [](terminalpp::element &elem,
               terminalpp::coordinate_type column,
               terminalpp::coordinate_type row)
            {
                elem = 'X';
            });

        scrollbar_->on_redraw.connect(
            [this](std::vector<terminalpp::rectangle> const &regions)
            {
                for (auto const &region : regions)
                {
                    scrollbar_->draw(surface_, region);
                }
            });
    }

    std::shared_ptr<munin::horizontal_scrollbar> scrollbar_ {
        munin::make_horizontal_scrollbar()
    };

    terminalpp::canvas canvas_{{16, 1}};
    munin::render_surface surface_{canvas_};
};

}

TEST_P(horizontal_scroll_bar_slider_position, draws_slider_at_the_expected_position)
{
    using std::get;

    auto const &param = GetParam();
    auto const &x_position = get<0>(param);
    auto const &width = get<1>(param);
    auto const &expected_slider_x_position = get<2>(param);

    scrollbar_->set_slider_position(x_position, width);

    for (terminalpp::coordinate_type x = 0; x < canvas_.size().width_; ++x)
    {
        if (x == expected_slider_x_position)
        {
            ASSERT_EQ(munin::detail::single_lined_cross, canvas_[x][0])
                << "Expected slider cross at position " << x;
        }
        else
        {
            ASSERT_EQ(munin::detail::single_lined_horizontal_beam, canvas_[x][0])
                << "Expected beam at position " << x;
        }
    }
}

static slider_test_data const test_data[] = {
    slider_test_data{ 0,  20, 0  },  // Special: 0
    slider_test_data{ 1,  20, 1  },  // 1
    slider_test_data{ 2,  20, 1  },  // 1 + 14/18 // (14 / 18 per step )
    slider_test_data{ 3,  20, 2  },  // 2 + 10/18
    slider_test_data{ 4,  20, 3  },  // 3 + 6/18

    slider_test_data{ 5,  20, 4  },  // 4 + 2/18
    slider_test_data{ 6,  20, 4  },  // 4 + 16/18
    slider_test_data{ 7,  20, 5  },  // 5 + 12/18
    slider_test_data{ 8,  20, 6  },  // 6 + 8/18
    slider_test_data{ 9,  20, 7  },  // 7 + 4/18

    slider_test_data{ 10, 20, 8  },  // 8 + 0/18
    slider_test_data{ 11, 20, 8  },  // 8 + 14/18
    slider_test_data{ 12, 20, 9  },  // 9 + 10/18
    slider_test_data{ 13, 20, 10 },  // 10 + 6/18
    slider_test_data{ 14, 20, 11 },  // 11 + 2/18

    slider_test_data{ 15, 20, 11 },  // 11 + 16/18
    slider_test_data{ 16, 20, 12 },  // 12 + 12/18
    slider_test_data{ 17, 20, 13 },  // 13 + 8/18
    slider_test_data{ 18, 20, 14 },  // 14 + 4/18

    slider_test_data{ 19, 20, 15 },  // Special: 15
};

INSTANTIATE_TEST_SUITE_P(
    setting_slider_position_shows_slider_in_expected_location,
    horizontal_scroll_bar_slider_position,
    ValuesIn(test_data)
);
