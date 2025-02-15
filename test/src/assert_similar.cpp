#include "assert_similar.hpp"

#include <gtest/gtest.h>
#include <terminalpp/algorithm/for_each_in_region.hpp>

void assert_similar_canvas_block(
    std::initializer_list<terminalpp::string const> expected,
    terminalpp::canvas const &cvs)
{
    assert_similar_canvas_block(std::span{expected}, cvs);
}

void assert_similar_canvas_block(
    std::span<terminalpp::string const> expected, terminalpp::canvas const &cvs)
{
    assert_similar_canvas_block(expected, cvs, {{}, cvs.size()});
}

void assert_similar_canvas_block(
    std::initializer_list<terminalpp::string const> expected,
    terminalpp::canvas const &cvs,
    terminalpp::rectangle const &bounds)
{
    assert_similar_canvas_block(std::span{expected}, cvs, bounds);
}

void assert_similar_canvas_block(
    std::span<terminalpp::string const> expected,
    terminalpp::canvas const &cvs,
    terminalpp::rectangle const &bounds)
{
    terminalpp::for_each_in_region(
        cvs,
        bounds,
        [&expected](
            terminalpp::element const &elem,
            terminalpp::coordinate_type column,
            terminalpp::coordinate_type row) {
            ASSERT_EQ(expected[row][column], elem)
                << "row = " << row << ", column = " << column;
        });
}
