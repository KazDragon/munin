#include "munin/detail/algorithm.hpp"

#include <gtest/gtest.h>

TEST(munin_algorithm, test_rectangle_intersection_same)
{
    // Test that the overlapping area of two similar rectangles is the same
    // as the area of the overlapping rectangles.
    terminalpp::rectangle lhs = {
        {1, 2},
        {3, 4}
    };
    terminalpp::rectangle rhs = lhs;

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(terminalpp::coordinate_type(1), intersection->origin_.x_);
    ASSERT_EQ(terminalpp::coordinate_type(2), intersection->origin_.y_);
    ASSERT_EQ(terminalpp::coordinate_type(3), intersection->size_.width_);
    ASSERT_EQ(terminalpp::coordinate_type(4), intersection->size_.height_);
}

TEST(munin_algorithm, test_rectangle_intersection_no_overlap)
{
    // Test that the intersection of two rectangles that do overlap is
    // returned as not-a-rectangle.
    terminalpp::rectangle lhs = {
        {0, 0},
        {1, 1}
    };
    terminalpp::rectangle rhs = {
        {1, 1},
        {1, 1}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_FALSE(intersection.has_value());
}

TEST(munin_algorithm, test_rectangle_intersection_overlap_top_left)
{
    // Test that, if the second rectangle overlaps the top left of the first
    // rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0+-----+         0
    // 1| R +----+      1    +-+
    // 2|   | L  |  ->  2    | |
    // 3+---|    |      3    +-+
    // 4    +----+      4
    //
    terminalpp::rectangle lhs = {
        {4, 1},
        {6, 4}
    };
    terminalpp::rectangle rhs = {
        {0, 0},
        {7, 4}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {4, 1},
            {3, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_overlap_bottom_left)
{
    // Test that, if the second rectangle overlaps the bottom left of the first
    // rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0    +----+      0
    // 1    | L  |      1
    // 2+---|    |  ->  2    +-+
    // 3| R +----+      3    +-+
    // 4+-----+         4
    terminalpp::rectangle lhs = {
        {4, 0},
        {5, 4}
    };
    terminalpp::rectangle rhs = {
        {0, 2},
        {7, 3}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {4, 2},
            {3, 2}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_overlap_bottom_right)
{
    // Test that, if the second rectangle overlaps the bottom right of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1 +----+         1
    // 2 | L  |--+      2   +--+
    // 3 |    |  |  ->  3   |  |
    // 4 +----+  |      4   +--+
    // 5   |   R |      5
    // 6   +-----+      6

    terminalpp::rectangle lhs = {
        {1, 1},
        {6, 4}
    };
    terminalpp::rectangle rhs = {
        {3, 2},
        {7, 5}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 2},
            {4, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_overlap_top_right)
{
    // Test that, if the second rectangle overlaps the top right of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1    +----+      1
    // 2    |  R |      2
    // 3  +----+ |  ->  3    +--+
    // 4  | L  | |      4    |  |
    // 5  |    |-+      5    +--+
    // 6  +----+        6

    terminalpp::rectangle lhs = {
        {2, 3},
        {6, 4}
    };
    terminalpp::rectangle rhs = {
        {4, 1},
        {6, 5}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {4, 3},
            {4, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_enclose_top)
{
    // Test that, if the second rectangle encloses the top of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1 +------+       1
    // 2 |   R  |       2
    // 3 | +--+ |   ->  3   +--+
    // 4 | | L| |       4   |  |
    // 5 +-|  |-+       5   +--+
    // 6   +--+         6

    terminalpp::rectangle lhs = {
        {3, 3},
        {4, 4}
    };
    terminalpp::rectangle rhs = {
        {1, 1},
        {8, 5}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 3},
            {4, 3}
    }));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);  // NOLINT
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 3},
            {4, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_enclose_left)
{
    // Test that, if the second rectangle encloses the left of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1 +----+         1
    // 2 | R  |         2
    // 3 | +-----+      3   +--+
    // 4 | | L   |  ->  4   |  |
    // 5 | +-----+      5   +--+
    // 6 |    |         6
    // 7 +----+         7

    terminalpp::rectangle lhs = {
        {3, 3},
        {7, 3}
    };
    terminalpp::rectangle rhs = {
        {1, 1},
        {6, 7}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 3},
            {4, 3}
    }));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);  // NOLINT
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 3},
            {4, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_enclose_bottom)
{
    // Test that, if the second rectangle encloses the bottom of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1  +---+         1
    // 2  |L  |         2
    // 3+-|   |--+  ->  3  +---+
    // 4| +---+ R|      4  +---+
    // 5+--------+      5

    terminalpp::rectangle lhs = {
        {0,  3},
        {10, 3}
    };
    terminalpp::rectangle rhs = {
        {2, 1},
        {5, 4}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {2, 3},
            {5, 2}
    }));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);  // NOLINT
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {2, 3},
            {5, 2}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_enclose_right)
{
    // Test that, if the second rectangle encloses the right of the
    // first rectangle, that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1     +---+      1
    // 2  +----+ |      2     +-+
    // 3  | L  | |  ->  3     | |
    // 4  +----+ |      4     +-+
    // 5     | R |      5
    // 6     +---+      6

    terminalpp::rectangle lhs = {
        {5, 1},
        {5, 6}
    };
    terminalpp::rectangle rhs = {
        {2, 2},
        {6, 3}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {5, 2},
            {3, 3}
    }));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);  // NOLINT
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {5, 2},
            {3, 3}
    }));
}

TEST(munin_algorithm, test_rectangle_intersection_contain)
{
    // Test that, if the second rectangle contains the first entirely,
    // that overlapping region is returned.
    //
    //  0123456789       0123456789
    // 0                0
    // 1 +------+       1
    // 2 | +--+ |       2   +--+
    // 3 | |L |R|   ->  3   |  |
    // 4 | +--+ |       4   +--+
    // 5 +------+       5

    terminalpp::rectangle lhs = {
        {3, 2},
        {4, 3}
    };
    terminalpp::rectangle rhs = {
        {1, 1},
        {8, 5}
    };

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 2},
            {4, 3}
    }));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);  // NOLINT
    ASSERT_TRUE(intersection.has_value());

    ASSERT_EQ(
        *intersection,
        terminalpp::rectangle({
            {3, 2},
            {4, 3}
    }));
}
