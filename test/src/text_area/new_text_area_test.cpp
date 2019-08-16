#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/string.hpp>

using testing::ValuesIn;

using a_new_text_area = a_text_area;

using namespace terminalpp::literals;

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

TEST_F(a_new_text_area, has_a_preferred_width_of_zero_and_a_preferred_size_of_one)
{
    // A text area always wants to put its cursor somewhere, and will use a
    // textel of height 1 if it can.
    ASSERT_EQ(terminalpp::extent(0, 1), text_area_.get_preferred_size());
}

TEST_F(a_new_text_area, draws_only_spaces)
{
    text_area_.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    text_area_.draw(surface, {{}, text_area_.get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
}

TEST_F(a_new_text_area, announces_new_caret_and_cursor_positions_and_preferred_size_when_inserting_text_at_the_caret)
{
    text_area_.set_size({2, 2});

    bool caret_position_changed = false;
    text_area_.on_caret_position_changed.connect(
        [&caret_position_changed]()
        {
            caret_position_changed = true;
        });

    bool cursor_position_changed = false;
    text_area_.on_cursor_position_changed.connect(
        [&cursor_position_changed]()
        {
            cursor_position_changed = true;
        });

    bool preferred_size_changed = false;
    text_area_.on_preferred_size_changed.connect(
        [&preferred_size_changed]()
        {
            preferred_size_changed = true;
        });

    text_area_.insert_text("a"_ts);

    ASSERT_TRUE(caret_position_changed);
    ASSERT_EQ(1, text_area_.get_caret_position());

    ASSERT_TRUE(cursor_position_changed);
    ASSERT_EQ(terminalpp::point(1, 0), text_area_.get_cursor_position());

    ASSERT_TRUE(preferred_size_changed);
    ASSERT_EQ(terminalpp::extent(1, 1), text_area_.get_preferred_size());
}

TEST_F(a_new_text_area, requests_a_redraw_and_draws_inserted_text_when_text_is_inserted)
{
    text_area_.set_size({2, 2});
    
    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });
    
    text_area_.insert_text("b"_ts);
    
    ASSERT_TRUE(redraw_requested);
    
    terminalpp::canvas canvas({3, 3});
    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'b'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
}

TEST_F(a_new_text_area, flows_long_text_into_the_next_line)
{
    text_area_.set_size({2, 2});
    
    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });
    
    text_area_.insert_text("cde"_ts);
    
    ASSERT_TRUE(redraw_requested);
    
    terminalpp::canvas canvas({3, 3});
    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'c'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
}

TEST_F(a_new_text_area, flows_newlines_when_drawing_text)
{
    text_area_.set_size({2, 2});
    
    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });
    
    text_area_.insert_text("c\nde"_ts);
    
    ASSERT_TRUE(redraw_requested);
    
    terminalpp::canvas canvas({3, 3});
    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'c'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{'X'}, canvas[2][2]);
}

using text_area_layout_data = std::tuple<
    terminalpp::string, // text content
    terminalpp::extent  // expected size
>;

class unjustified_text_areas 
  : public testing::TestWithParam<text_area_layout_data>
{
};

TEST_P(unjustified_text_areas, have_a_preferred_size_related_to_text_content)
{
    auto const &param = GetParam();
    auto const &text = std::get<0>(param);
    auto const &expected_size = std::get<1>(param);
    
    munin::text_area text_area;
    text_area.insert_text(text);
    
    ASSERT_EQ(expected_size, text_area.get_preferred_size());
};

INSTANTIATE_TEST_CASE_P(
    unjustified_text_areas_behave,
    unjustified_text_areas,
    ValuesIn
    ({
        text_area_layout_data{"a"_ts, {1, 1}},
        text_area_layout_data{"ab"_ts, {2, 1}},
        text_area_layout_data{"ab\n"_ts, {2, 2}},
        text_area_layout_data{"ab\nc"_ts, {2, 2}},
    })
);
