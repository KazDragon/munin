#pragma once

#include <terminalpp/rectangle.hpp>
#include <vector>

void assert_equivalent_redraw_regions(
    std::vector<terminalpp::rectangle> const &lhs,
    std::vector<terminalpp::rectangle> const &rhs);