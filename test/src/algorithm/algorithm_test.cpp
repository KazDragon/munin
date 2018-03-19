#include "munin/detail/algorithm.hpp"
#include <gtest/gtest.h>

TEST(munin_algorithm, test_rectangle_intersection_same)
{
    // Test that the overlapping area of two similar rectangles is the same
    // as the area of the overlapping rectangles.
    munin::rectangle lhs = {{1, 2}, {3, 4}};
    munin::rectangle rhs = lhs;

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(terminalpp::coordinate_type(1), intersection->origin.x);
    ASSERT_EQ(terminalpp::coordinate_type(2), intersection->origin.y);
    ASSERT_EQ(terminalpp::coordinate_type(3), intersection->size.width);
    ASSERT_EQ(terminalpp::coordinate_type(4), intersection->size.height);
}

TEST(munin_algorithm, test_rectangle_intersection_no_overlap)
{
    // Test that the intersection of two rectangles that do overlap is
    // returned as not-a-rectangle.
    munin::rectangle lhs = {{0, 0}, {1, 1}};
    munin::rectangle rhs = {{1, 1}, {1, 1}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_FALSE(intersection.is_initialized());
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
    munin::rectangle lhs = {{4, 1}, {6, 4}};
    munin::rectangle rhs = {{0, 0}, {7, 4}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{4, 1}, {3, 3}}));
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
    munin::rectangle lhs = {{4, 0}, {5, 4}};
    munin::rectangle rhs = {{0, 2}, {7, 3}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{4, 2}, {3, 2}}));
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

    munin::rectangle lhs = {{1, 1}, {6, 4}};
    munin::rectangle rhs = {{3, 2}, {7, 5}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 2}, {4, 3}}));
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

    munin::rectangle lhs = {{2, 3}, {6, 4}};
    munin::rectangle rhs = {{4, 1}, {6, 5}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{4, 3}, {4, 3}}));
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

    munin::rectangle lhs = {{3, 3}, {4, 4}};
    munin::rectangle rhs = {{1, 1}, {8, 5}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 3}, {4, 3}}));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 3}, {4, 3}}));
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

    munin::rectangle lhs = {{3, 3}, {7, 3}};
    munin::rectangle rhs = {{1, 1}, {6, 7}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 3}, {4, 3}}));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 3}, {4, 3}}));
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

    munin::rectangle lhs = {{0, 3}, {10, 3}};
    munin::rectangle rhs = {{2, 1}, {5, 4}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{2, 3}, {5, 2}}));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{2, 3}, {5, 2}}));
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

    munin::rectangle lhs = {{5, 1}, {5, 6}};
    munin::rectangle rhs = {{2, 2}, {6, 3}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{5, 2}, {3, 3}}));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{5, 2}, {3, 3}}));
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

    munin::rectangle lhs = {{3, 2}, {4, 3}};
    munin::rectangle rhs = {{1, 1}, {8, 5}};

    auto intersection = munin::detail::intersection(lhs, rhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 2}, {4, 3}}));

    // Ensure that this works for the opposite case too, where the first
    // rectangle encloses the second.
    intersection = munin::detail::intersection(rhs, lhs);
    ASSERT_TRUE(intersection.is_initialized());

    ASSERT_EQ(*intersection, munin::rectangle({{3, 2}, {4, 3}}));
}

TEST(munin_algorithm, test_rectangular_slice)
{
    {
        // Test the identity case.  The following should be returned unchanged.
        std::vector<munin::rectangle> rectangles = {
            {{0, 0}, {1, 1}},
            {{1, 1}, {1, 1}},
            {{2, 2}, {1, 1}}
        };

        auto result = munin::detail::rectangular_slice(rectangles);

        ASSERT_EQ(rectangles.size(), result.size());

        for (size_t index = 0; index < rectangles.size(); ++index)
        {
            ASSERT_EQ(rectangles[index], result[index]);
        }
    }

    {
        // Test the simple case.  One rectangle of two lines should be sliced
        // into two rectangles of one line.
        std::vector<munin::rectangle> rectangles = {
            {{0, 0}, {1, 2}}
        };

        std::vector<munin::rectangle> expected_rectangles = {
            {{0, 0}, {1, 1}},
            {{0, 1}, {1, 1}}
        };

        auto result = munin::detail::rectangular_slice(rectangles);

        ASSERT_EQ(expected_rectangles.size(), result.size());

        for (size_t index = 0; index < expected_rectangles.size(); ++index)
        {
            ASSERT_EQ(expected_rectangles[index], result[index]);
        }
    }

    {
        // Test the adjacent case.
        std::vector<munin::rectangle> rectangles = {
            {{0, 0}, {1, 1}},
            {{1, 0}, {1, 1}}
        };

        std::vector<munin::rectangle> expected_rectangles = {
            {{0, 0}, {2, 1}}
        };

        auto result = munin::detail::rectangular_slice(rectangles);

        ASSERT_EQ(expected_rectangles.size(), result.size());

        for (size_t index = 0; index < expected_rectangles.size(); ++index)
        {
            ASSERT_EQ(expected_rectangles[index], result[index]);
        }
    }

    {
        // Test the overlap case.  After being sliced, rectangles should be
        // merged if they occupy common space.

        //
        //  +---+         +---+
        //  |   |         |   |
        //  | +-+-+       +---+-+
        //  | |   |   ->  |     |
        //  +-+   |       +-+---+
        //    |   |         |   |
        //    +---+         +---+

        std::vector<munin::rectangle> rectangles = {
            {{0, 0}, {2, 2}},
            {{1, 1}, {2, 2}}
        };

        std::vector<munin::rectangle> expected_rectangles = {
            {{0, 0}, {2, 1}},
            {{0, 1}, {3, 1}},
            {{1, 2}, {2, 1}}
        };

        auto result = munin::detail::rectangular_slice(rectangles);

        ASSERT_EQ(expected_rectangles.size(), result.size());

        for (size_t index = 0; index < expected_rectangles.size(); ++index)
        {
            ASSERT_EQ(expected_rectangles[index], result[index]);
        }
    }
    {
        // Test the unsorted case.  This is the same as before, except the
        // order of the rectangles is in reverse.  The result should be the
        // same.

        std::vector<munin::rectangle> rectangles = {
            {{1, 1}, {2, 2}},
            {{0, 0}, {2, 2}}
        };

        std::vector<munin::rectangle> expected_rectangles = {
            {{0, 0}, {2, 1}},
            {{0, 1}, {3, 1}},
            {{1, 2}, {2, 1}}
        };

        auto result = munin::detail::rectangular_slice(rectangles);

        ASSERT_EQ(expected_rectangles.size(), result.size());

        for (size_t index = 0; index < expected_rectangles.size(); ++index)
        {
            ASSERT_EQ(expected_rectangles[index], result[index]);
        }
    }
}
