#pragma once

#include "munin/export.hpp"
#include "munin/render_surface_capabilities.hpp"

#include <terminalpp/canvas.hpp>

namespace munin {

//* =========================================================================
/// \brief A sub-view into a canvas that only allows reading and writing
/// to a its elements; not operations that would affect the entire canvas.
//* =========================================================================
class MUNIN_EXPORT render_surface
{
public:
    using size_type = terminalpp::coordinate_type;

    //* =====================================================================
    /// \brief A proxy into a column of elements on the canvas
    //* =====================================================================
    class MUNIN_EXPORT column_proxy
    {
    public:
        using size_type = terminalpp::coordinate_type;

        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        column_proxy(
            render_surface &surface, terminalpp::coordinate_type column);

        // ==================================================================
        // OPERATOR[]
        // ==================================================================
        terminalpp::element &operator[](terminalpp::coordinate_type row);

    private:
        render_surface &surface_;
        terminalpp::coordinate_type column_;
    };

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit render_surface(terminalpp::canvas &cvs);

    //* =====================================================================
    //\ brief Constructor with explicit render surface capabilities
    //* =====================================================================
    render_surface(
        terminalpp::canvas &cvs,
        render_surface_capabilities const &capabilities);

    //* =====================================================================
    /// \brief Returns true if the surface is known to support unicode.
    /// characters.  Attempting to render unicode on surfaces that do not
    /// support unicode may have unexpected results.
    //* =====================================================================
    [[nodiscard]] bool supports_unicode() const;

    //* ==== =================================================================
    /// \brief Offsets the canvas by a certain amount, causing it to become
    /// a view with the offset location as a basis.
    //* =====================================================================
    void offset_by(terminalpp::extent offset);

    //* =====================================================================
    /// \brief Returns the size of the canvas.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent size() const;

    //* =====================================================================
    /// \brief A subscript operator into a column
    //* =====================================================================
    column_proxy operator[](terminalpp::coordinate_type column);

private:
    //* =====================================================================
    /// \brief Gets an element from the underlying canvas.
    //* =====================================================================
    terminalpp::element &get_element(
        terminalpp::coordinate_type column, terminalpp::coordinate_type row);

    render_surface_capabilities const &capabilities_;
    terminalpp::canvas &canvas_;
    terminalpp::extent offset_;
};

}  // namespace munin
