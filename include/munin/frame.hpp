#pragma once

#include "munin/composite_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A base class for frames that exposes the border widths
//* =========================================================================
class MUNIN_EXPORT frame : public composite_component
{
public :    
    //* =====================================================================
    /// \brief returns the height of the north border.
    //* =====================================================================
    virtual terminalpp::coordinate_type north_border_height() const = 0;

    //* =====================================================================
    /// \brief returns the height of the south border.
    //* =====================================================================
    virtual terminalpp::coordinate_type south_border_height() const = 0;
    
    //* =====================================================================
    /// \brief returns the width of the west border.
    //* =====================================================================
    virtual terminalpp::coordinate_type west_border_width() const = 0;
    
    //* =====================================================================
    /// \brief returns the width of the east border.
    //* =====================================================================
    virtual terminalpp::coordinate_type east_border_width() const = 0;
};

}
