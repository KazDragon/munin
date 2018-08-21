#pragma once

#include <munin/rectangle.hpp>
#include <vector>

void assert_equivalent_redraw_regions(
    std::vector<munin::rectangle> const &lhs,
    std::vector<munin::rectangle> const &rhs);