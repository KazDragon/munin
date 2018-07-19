#pragma once

#include "munin/export.hpp"
#include "munin/render_surface_capabilities.hpp"
#include <terminalpp/canvas.hpp>
#include <iosfwd>

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
        row_proxy &operator=(row_proxy const &other);

        // ==================================================================
        // OPERATOR=
        // ==================================================================
        row_proxy &operator=(terminalpp::element const &value);

        // ==================================================================
        // CONVERSION OPERATOR: ELEMENT
        // ==================================================================
        operator terminalpp::element &();

        // ==================================================================
        // CONVERSION OPERATOR: ELEMENT
        // ==================================================================
        operator terminalpp::element const &() const;

        // ==================================================================
        // EQUALITY: ELEMENT
        // ==================================================================
        bool operator==(const terminalpp::element& value) const;
        
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
    /// \brief A constant proxy into a column of elements on the canvas
    //* =====================================================================
    class const_column_proxy
    {
    public :
        // ==================================================================
        // CONSTRUCTOR
        // ==================================================================
        const_column_proxy(
            render_surface const &surface, 
            terminalpp::coordinate_type column);

        // ==================================================================
        // OPERATOR[]
        // ==================================================================
        terminalpp::element const &operator[](
            terminalpp::coordinate_type row) const;

    private :
        render_surface const &surface_;
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

    //* =====================================================================
    /// \brief A subscript operator into a column
    //* =====================================================================
    const_column_proxy operator[](terminalpp::coordinate_type column) const;

private :
    //* =====================================================================
    /// \brief Set the value of an element.
    //* =====================================================================
    void set_element(
        terminalpp::coordinate_type column, 
        terminalpp::coordinate_type row, 
        terminalpp::element const &value);

    //* =====================================================================
    /// \brief Get the value of an element.
    //* =====================================================================
    terminalpp::element &get_element(
        terminalpp::coordinate_type column, 
        terminalpp::coordinate_type row);

    //* =====================================================================
    /// \brief Get the value of an element.
    //* =====================================================================
    terminalpp::element const &get_element(
        terminalpp::coordinate_type column, 
        terminalpp::coordinate_type row) const;

    render_surface_capabilities const &capabilities_;
    terminalpp::canvas &canvas_;
    terminalpp::extent  offset_;
};

MUNIN_EXPORT
std::ostream &operator<<(std::ostream &out, render_surface::row_proxy const &row);

}
