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

