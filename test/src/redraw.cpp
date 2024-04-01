#include "redraw.hpp"

#include <gtest/gtest.h>

#include <numeric>

namespace {

terminalpp::extent calculate_bounds(
    std::vector<terminalpp::rectangle> const &regions)
{
    return std::accumulate(
        regions.begin(),
        regions.end(),
        terminalpp::extent{},
        [](auto &bounds, auto const &region) {
            bounds.width_ = std::max(
                bounds.width_, region.origin_.x_ + region.size_.width_);
            bounds.height_ = std::max(
                bounds.height_, region.origin_.y_ + region.size_.height_);
            return bounds;
        });
}

std::vector<bool> create_redraw_map(
    std::vector<terminalpp::rectangle> const &regions)
{
    auto extent = calculate_bounds(regions);
    std::vector<bool> redraw_map(extent.width_ * extent.height_);

    for (auto const &region : regions)
    {
        for (auto row = region.origin_.y_;
             row < region.origin_.y_ + region.size_.height_;
             ++row)
        {
            for (auto col = region.origin_.x_;
                 col < region.origin_.x_ + region.size_.width_;
                 ++col)
            {
                redraw_map[(row * extent.width_) + col] = true;
            }
        }
    }

    return redraw_map;
}

}  // namespace

void assert_equivalent_redraw_regions(
    std::vector<terminalpp::rectangle> const &lhs,
    std::vector<terminalpp::rectangle> const &rhs)
{
    auto const lhs_bounds = calculate_bounds(lhs);
    auto const rhs_bounds = calculate_bounds(rhs);

    ASSERT_EQ(lhs_bounds, rhs_bounds);

    auto const lhs_map = create_redraw_map(lhs);
    auto const rhs_map = create_redraw_map(rhs);

    ASSERT_EQ(lhs_map, rhs_map);
}
