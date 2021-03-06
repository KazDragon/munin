#include "text_area_test.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>

terminalpp::element const a_text_area::fill = {'X'};

void a_text_area::fill_canvas(terminalpp::extent canvas_size)
{
    canvas_ = terminalpp::canvas{canvas_size};
    
    terminalpp::for_each_in_region(
        canvas_,
        {{}, canvas_size},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = a_text_area::fill;
        });
}

void a_text_area::verify_oob_is_untouched()
{
    auto const in_bounds = text_area_.get_size();
    auto const out_of_bounds = canvas_.size();
    
    // For now, assume IB and OOB are both based at {0,0}.
    for (auto oob_y = terminalpp::coordinate_type{0};
         oob_y < in_bounds.height_; 
         ++oob_y)
    {
        for (auto oob_x = in_bounds.width_; oob_x < out_of_bounds.width_; ++oob_x)
        {
            ASSERT_EQ(canvas_[oob_x][oob_y], a_text_area::fill);
        }
    }

    for (auto oob_y = in_bounds.height_; oob_y < out_of_bounds.height_; ++oob_y)
    {
        for (auto oob_x = terminalpp::coordinate_type{0};
             oob_x < out_of_bounds.width_;
             ++oob_x)
        {
            ASSERT_EQ(canvas_[oob_x][oob_y], a_text_area::fill);
        }
    }
}

TEST_F(a_text_area, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_text_area();
}
