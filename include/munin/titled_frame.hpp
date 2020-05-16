#pragma once

#include "munin/frame.hpp"
#include <terminalpp/string.hpp>

namespace munin {
    
//* =========================================================================
/// \brief A component that draws a frame around its edges, and leaves the
/// rest of the canvas undrawn.
///
/// The frame has two attributes, the lowlight and highlight attributes, and
/// these determine the attributes with which the frame is drawn.  These
/// attributes can be toggled by setting the highlight property.
//* =========================================================================
class MUNIN_EXPORT titled_frame : public frame
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit titled_frame(terminalpp::string const &title);
    
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~titled_frame();
    
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

protected :
    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;

    //* =====================================================================
    /// \brief Called when the focus of the associated component has changed
    /// Derived classes must override this to provide appropriate redraw
    /// callbacks so that the frame can be redrawn with this new focus.
    //* =====================================================================
    void do_inner_focus_changed() override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Creates a new titled frame
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<titled_frame> make_titled_frame(
    terminalpp::string const &title);

//* =========================================================================
/// \brief Creates a new titled frame that is automatically associated
/// with the passed component.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<titled_frame> make_titled_frame(
    terminalpp::string const &title,
    std::shared_ptr<component> const &associated_component);

}
