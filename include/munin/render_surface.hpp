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
public :
    //* =====================================================================
    /// \brief A proxy into a row of elements on the canvas
    //* =====================================================================
    class row_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        row_proxy(
            render_surface &surface, 
            terminalpp::coordinate_type column, 
            terminalpp::coordinate_type row);

        // ==================================================================
        // OPERATOR=
        // ==================================================================
        row_proxy &operator=(terminalpp::element const &value);

    private :
        render_surface &surface_;
        terminalpp::coordinate_type column_;
        terminalpp::coordinate_type row_;
    };

    //* =====================================================================
    /// \brief A proxy into a column of elements on the canvas
    //* =====================================================================
    class column_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        column_proxy(
            render_surface &surface, 
            terminalpp::coordinate_type column);

        // ==================================================================
        // OPERATOR[]
        // ==================================================================
        row_proxy operator[](terminalpp::coordinate_type row);

    private :
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
    bool supports_unicode() const;

    //* ==== =================================================================
    /// \brief Offsets the canvas by a certain amount, causing it to become
    /// a view with the offset location as a basis.
    //* =====================================================================
    void offset_by(terminalpp::extent offset);

    //* =====================================================================
    /// \brief Returns the size of the canvas.
    //* =====================================================================
    terminalpp::extent size() const;

    //* =====================================================================
    /// \brief A subscript operator into a column
    //* =====================================================================
    column_proxy operator[](terminalpp::coordinate_type column);

private :
    //* =====================================================================
    /// \brief Set the value of an element.
    //* =====================================================================
    void set_element(
        terminalpp::coordinate_type column, 
        terminalpp::coordinate_type row, 
        terminalpp::element const &value);

    render_surface_capabilities const &capabilities_;
    terminalpp::canvas &canvas_;
    terminalpp::extent  offset_;
};

}
