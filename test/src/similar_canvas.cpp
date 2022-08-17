#include "similar_canvas.hpp"

void assert_similar_canvas(
    std::vector<terminalpp::string> const &expected,
    terminalpp::canvas const &cvs)
{
    terminalpp::for_each_in_region(
        cvs,
        {{}, cvs.size()},
        [&expected](terminalpp::element const &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            ASSERT_EQ(expected[row][column], elem)
                << "row = " << row << ", column = " << column;
        });
}

