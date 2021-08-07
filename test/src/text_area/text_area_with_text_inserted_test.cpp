#include "text_area_test.hpp"
#include <munin/render_surface.hpp>

class a_text_area_with_text_inserted : public a_text_area
{
public:
    a_text_area_with_text_inserted()
    {
        text_area_.insert_text("ab");
    }
};

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_at_the_front)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("c", 0);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'c'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[2][0]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_in_the_middle)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("c", 1);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[2][0]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_that_pushes_existing_text_into_the_next_line)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("defg", 1);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{'f'}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{'g'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, lays_out_its_text_when_the_size_changes)
{
    text_area_.set_size({4, 2});

    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};

    text_area_.draw(surface, {{0, 0}, {4, 2}});

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);

    auto const expected_cursor_position = terminalpp::point{2, 0};
    ASSERT_EQ(expected_cursor_position, text_area_.get_cursor_position());

    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, announces_a_change_in_preferred_size_if_its_size_is_changed_such_that_there_are_now_more_lines)
{
    // for example, if I change the size of "ab" to 1x1 such that the b would
    // now bleed into the next line, the text area should request the new
    // preferred height.
    text_area_.set_size({2, 1});

    auto preferred_size = terminalpp::extent{};
    text_area_.on_preferred_size_changed.connect(
        [&]
        {
            preferred_size = text_area_.get_preferred_size();
        });

    text_area_.set_size({1, 1});

    auto const expected_preferred_size = terminalpp::extent{1, 3};
    ASSERT_EQ(expected_preferred_size, preferred_size);
}
