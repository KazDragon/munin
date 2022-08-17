#pragma once

#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/string.hpp>
#include <vector>
#include <gtest/gtest.h>

void assert_similar_canvas_block(
    std::vector<terminalpp::string> const &expected,
    terminalpp::canvas const &cvs,
    terminalpp::rectangle const bounds)
{
    terminalpp::for_each_in_region(
        cvs,
        bounds,
        [&expected](terminalpp::element const &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            ASSERT_EQ(expected[row][column], elem)
                << "row = " << row << ", column = " << column;
        });
}

