#pragma once

#include "munin/frame.hpp"
#include <terminalpp/attribute.hpp>

namespace munin {
    
//* =========================================================================
/// \brief A component that draws a frame around its edges, and leaves the
/// rest of the canvas undrawn.
///
/// The frame has two attributes, the lowlight and highlight attributes, and
/// these determine the attributes with which the frame is drawn.  These
/// attributes can be toggled by setting the highlight property.
//* =========================================================================
class MUNIN_EXPORT solid_frame : public frame
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    solid_frame();
    
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~solid_frame();
    
    //* =====================================================================
    /// \brief Sets the highlight attribute.
    //* =====================================================================
    void set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute);
        
    //* =====================================================================
    /// \brief Sets the lowlight attribute.
    //* =====================================================================
    void set_lowlight_attribute(
        terminalpp::attribute const &lowlight_attribute);
    
    //* =====================================================================
    /// \brief Associates a component with this frame, such that its focus
    /// state is reflected by the highlight state of this frame.
    //* =====================================================================
    void highlight_on_focus(
        std::shared_ptr<component> const &associated_component);

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

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Creates a new solid frame
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<solid_frame> make_solid_frame();

//* =========================================================================
/// \brief Creates a new solid frame that is automatically associated
/// with the passed component.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<solid_frame> make_solid_frame(
    std::shared_ptr<component> const &associated_component);

}
