#pragma once

#include "munin/export.hpp"
#include <terminalpp/point.hpp>
#include <terminalpp/extent.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that represents a rectangle in space.
//* =========================================================================
struct rectangle
{
    //* =====================================================================
    /// \brief Default Constructor
    /// \par
    /// Constructs the rectangle, leaving the origin and size uninitialised.
    //* =====================================================================
    constexpr rectangle() = default;

    //* =====================================================================
    /// \brief Constructor
    /// \par
    /// Constructs the rectangle, using the specified origin and size.
    //* =====================================================================
    constexpr rectangle(terminalpp::point org, terminalpp::extent sz)
      : origin(org),
        size(sz)
    {
    }

    terminalpp::point  origin;
    terminalpp::extent size;
};

// ==========================================================================
// OPERATOR+(POINT,EXTENT)
// ==========================================================================
constexpr rectangle operator+(
    terminalpp::point const &pt, terminalpp::extent const &ext)
{
    return rectangle(pt, ext);
}

// ==========================================================================
// OPERATOR==(RECTANGLE,RECTANGLE)
// ==========================================================================
constexpr bool operator==(rectangle const &lhs, rectangle const &rhs)
{
    return lhs.origin == rhs.origin && lhs.size == rhs.size;
}

// ==========================================================================
// OPERATOR!=(RECTANGLE,RECTANGLE)
// ==========================================================================
constexpr bool operator!=(rectangle const &lhs, rectangle const &rhs)
{
    return !(lhs == rhs);
}

}
