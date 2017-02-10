#pragma once

#include "munin/export.hpp"
#include "munin/rectangle.hpp"
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace terminalpp {
    class canvas;
    class canvas_view;
}

namespace munin {

//* =========================================================================
/// \brief Returns the intersection of two rectangles.
//* =========================================================================
MUNIN_EXPORT 
boost::optional<rectangle> intersection(
    rectangle const &lhs
  , rectangle const &rhs);

//* =========================================================================
/// \brief Returns an array of sliced rectangles.
/// \par
/// A rectangular slice takes an array of rectangles, and returns an array
/// of the fewest number of rectangles, each of height 1, that describes the
/// area covered by the original rectangles.  These are sorted from left to
/// right, top to bottom.
/// \todo Consider a span interface.
//* =========================================================================
MUNIN_EXPORT 
std::vector<rectangle> rectangular_slice(
    std::vector<rectangle> const &rectangles);

//* =========================================================================
/// \brief Returns an array of clipped regions.
/// \par
/// A clipped region is one whose bounds are trimmed to a particular size.
/// For example, if the size is (5,5), and a region is (3,2)->[3,3], then
/// the region is clipped to (3,2)->[2,3].
/// \todo Consider a span interface.
//* =========================================================================
std::vector<rectangle> clip_regions(
    std::vector<rectangle> regions
  , terminalpp::extent     size);

//* =========================================================================
/// \brief Returns the passed array, except that any regions that have a
/// height or width of 0 are omitted.
/// \todo Consider a span interface.
//* =========================================================================
std::vector<rectangle> prune_regions(std::vector<rectangle> regions);

//* =========================================================================
/// \brief Copies a region from one canvas to another.
//* =========================================================================
MUNIN_EXPORT
void copy_region(
    rectangle               const &region
  , terminalpp::canvas      const &source
  , terminalpp::canvas_view       &destination);

}
