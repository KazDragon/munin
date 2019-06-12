#pragma once

#include "munin/export.hpp"
#include <terminalpp/rectangle.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace terminalpp {
    class canvas;
    class canvas_view;
}

namespace munin { namespace detail {

//* =========================================================================
/// \brief Returns the intersection of two rectangles.
//* =========================================================================
MUNIN_EXPORT 
boost::optional<terminalpp::rectangle> intersection(
    terminalpp::rectangle const &lhs
  , terminalpp::rectangle const &rhs);

}}
