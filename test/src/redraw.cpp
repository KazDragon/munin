#include "redraw.hpp"
#include <numeric>
#include <gtest/gtest.h>

static terminalpp::extent calculate_bounds(
    std::vector<munin::rectangle> const &regions)
{
    return std::accumulate(
        regions.begin(),
        regions.end(),
        terminalpp::extent{},
        [](auto &bounds, auto const &region)
        {
            bounds.width = std::max(bounds.width, region.origin.x + region.size.width);
            bounds.height = std::max(bounds.height, region.origin.y + region.size.height);
            return bounds;
        });
}

static std::vector<bool> create_redraw_map(
    std::vector<munin::rectangle> const &regions)
{
    auto extent = calculate_bounds(regions);
    std::vector<bool> redraw_map(extent.width * extent.height);
    
    for(auto const &region : regions)
    {
        for (auto row = region.origin.y; row < region.origin.y + region.size.height; ++row)
        {
            for (auto col = region.origin.x; col < region.origin.x + region.size.width; ++col)
            {
                redraw_map[(row * extent.width) + col] = true;
            }
        }
    }
    
    return redraw_map;
}

void assert_equivalent_redraw_regions(
    std::vector<munin::rectangle> const &lhs,
    std::vector<munin::rectangle> const &rhs)
{
    auto const lhs_bounds = calculate_bounds(lhs);
    auto const rhs_bounds = calculate_bounds(rhs);

    ASSERT_EQ(lhs_bounds, rhs_bounds); 

    auto const lhs_map = create_redraw_map(lhs);
    auto const rhs_map = create_redraw_map(rhs);
    
    ASSERT_EQ(lhs_map, rhs_map);
}
