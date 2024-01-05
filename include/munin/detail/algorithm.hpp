#pragma once

#include "munin/export.hpp"
#include <terminalpp/rectangle.hpp>
#include <optional>
#include <string>
#include <vector>

namespace terminalpp {
class canvas;
class canvas_view;
}  // namespace terminalpp

namespace munin::detail {

//* =========================================================================
/// \brief Returns the intersection of two rectangles.
//* =========================================================================
MUNIN_EXPORT
std::optional<terminalpp::rectangle> intersection(
    terminalpp::rectangle const &lhs, terminalpp::rectangle const &rhs);

}  // namespace munin::detail
