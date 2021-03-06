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
    auto const &leftmost     = lhs.origin_.x_ < rhs.origin_.x_ ? lhs : rhs;
    auto const &not_leftmost = lhs.origin_.x_ < rhs.origin_.x_ ? rhs : lhs;

    // Calculate the rectangle with the topmost origin.
    auto const &topmost        = lhs.origin_.y_ < rhs.origin_.y_ ? lhs : rhs;
    auto const &not_topmost    = lhs.origin_.y_ < rhs.origin_.y_ ? rhs : lhs;

    bool overlaps =
        not_leftmost.origin_.x_ >= leftmost.origin_.x_
     && not_leftmost.origin_.x_ <  leftmost.origin_.x_ + leftmost.size_.width_
     && not_topmost.origin_.y_  >= topmost.origin_.y_
     && not_topmost.origin_.y_  <  topmost.origin_.y_ + topmost.size_.height_;

    if (!overlaps)
    {
        // There is no overlapping rectangle.
        return {};
    }

    // The overlapping rectangle has an origin that starts at the same x-
    // co-ordinate as the not-leftmost rectangle, and at the same y-co-ordinate
    // as the not-topmost rectangle.
    terminalpp::rectangle overlap;
    overlap.origin_.x_ = not_leftmost.origin_.x_;
    overlap.origin_.y_ = not_topmost.origin_.y_;

    // If the leftmost rectangle completely contains the other rectangle,
    // then the width is simply that of the enclosed rectangle.
    if (leftmost.origin_.x_ + leftmost.size_.width_
      > not_leftmost.origin_.x_ + not_leftmost.size_.width_)
    {
        overlap.size_.width_ = not_leftmost.size_.width_;
    }
    // Otherwise, the calculated width is the width of the leftmost rectangle
    // minus the difference between the origins of the two rectangles.
    else
    {
        overlap.size_.width_ =
            leftmost.size_.width_ - (not_leftmost.origin_.x_ - leftmost.origin_.x_);
    }

    // Same as above, but with height instead of width.
    if (topmost.origin_.y_ + topmost.size_.height_
      > not_topmost.origin_.y_ + not_topmost.size_.height_)
    {
        overlap.size_.height_ = not_topmost.size_.height_;
    }
    else
    {
        overlap.size_.height_ =
            topmost.size_.height_ - (not_topmost.origin_.y_ - topmost.origin_.y_);
    }

    return overlap;
}

}}

