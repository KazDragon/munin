#pragma once

#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/string.hpp>
#include <vector>
#include <gtest/gtest.h>

void assert_similar_canvas(
    std::vector<terminalpp::string> const &expected,
    terminalpp::canvas const &cvs);
