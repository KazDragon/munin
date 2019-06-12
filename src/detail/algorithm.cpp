#include "munin/detail/algorithm.hpp"
#include <terminalpp/canvas.hpp>
#include <algorithm>

namespace munin { namespace detail {

// ==========================================================================
// INTERSECTION
// ==========================================================================
boost::optional<terminalpp::rectangle> intersection(
    terminalpp::rectangle const &lhs, terminalpp::rectangle const &rhs)
{
    // Check to see if the rectangles overlap.

    // Calculate the rectangle with the leftmost origin, and its counterpart.
    // Note: the rectangle that is not the leftmost is not necessarily the
    // rightmost, since the leftmost rectangle may extend further than its
    // counterpart.
    auto const &leftmost     = lhs.origin.x < rhs.origin.x ? lhs : rhs;
    auto const &not_leftmost = lhs.origin.x < rhs.origin.x ? rhs : lhs;

    // Calculate the rectangle with the topmost origin.
    auto const &topmost        = lhs.origin.y < rhs.origin.y ? lhs : rhs;
    auto const &not_topmost    = lhs.origin.y < rhs.origin.y ? rhs : lhs;

    bool overlaps =
        not_leftmost.origin.x >= leftmost.origin.x
     && not_leftmost.origin.x <  leftmost.origin.x + leftmost.size.width
     && not_topmost.origin.y  >= topmost.origin.y
     && not_topmost.origin.y  <  topmost.origin.y + topmost.size.height;

    if (!overlaps)
    {
        // There is no overlapping rectangle.
        return {};
    }

    // The overlapping rectangle has an origin that starts at the same x-
    // co-ordinate as the not-leftmost rectangle, and at the same y-co-ordinate
    // as the not-topmost rectangle.
    terminalpp::rectangle overlap;
    overlap.origin.x = not_leftmost.origin.x;
    overlap.origin.y = not_topmost.origin.y;

    // If the leftmost rectangle completely contains the other rectangle,
    // then the width is simply that of the enclosed rectangle.
    if (leftmost.origin.x + leftmost.size.width
      > not_leftmost.origin.x + not_leftmost.size.width)
    {
        overlap.size.width = not_leftmost.size.width;
    }
    // Otherwise, the calculated width is the width of the leftmost rectangle
    // minus the difference between the origins of the two rectangles.
    else
    {
        overlap.size.width =
            leftmost.size.width - (not_leftmost.origin.x - leftmost.origin.x);
    }

    // Same as above, but with height instead of width.
    if (topmost.origin.y + topmost.size.height
      > not_topmost.origin.y + not_topmost.size.height)
    {
        overlap.size.height = not_topmost.size.height;
    }
    else
    {
        overlap.size.height =
            topmost.size.height - (not_topmost.origin.y - topmost.origin.y);
    }

    return overlap;
}

}}

