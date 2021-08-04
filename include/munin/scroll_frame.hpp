#pragma once
#include "munin/frame.hpp"
#include <memory>

namespace munin {

//* =========================================================================
/// \brief A class that frames a viewport using scrollbars.
//* =========================================================================
class MUNIN_EXPORT scroll_frame : public frame
{
public:
    //* =====================================================================
    /// \brief returns the height of the north border.
    //* =====================================================================
    terminalpp::coordinate_type north_border_height() const override;

    //* =====================================================================
    /// \brief returns the height of the south border.
    //* =====================================================================
    terminalpp::coordinate_type south_border_height() const override;
    
    //* =====================================================================
    /// \brief returns the width of the west border.
    //* =====================================================================
    terminalpp::coordinate_type west_border_width() const override;
    
    //* =====================================================================
    /// \brief returns the width of the east border.
    //* =====================================================================
    terminalpp::coordinate_type east_border_width() const override;

protected:
    //* =====================================================================
    /// \brief Called when the focus of the associated component has changed
    /// Derived classes must override this to provide appropriate redraw
    /// callbacks so that the frame can be redrawn with this new focus.
    //* =====================================================================
    void do_inner_focus_changed() override;
};

//* =========================================================================
/// \brief Returns a newly-created scroll frame.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<scroll_frame> make_scroll_frame();

}