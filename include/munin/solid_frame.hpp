#pragma once

#include <munin/composite_component.hpp>

namespace munin {
    
//* =========================================================================
/// \brief A component that draws a frame around its edges, and leaves the
/// rest of the canvas undrawn.
///
/// The frame has two attributes, the lowlight and highlight attributes, and
/// these determine the attributes with which the frame is drawn.  These
/// attributes can be toggled by setting the highlight property.
//* =========================================================================
class MUNIN_EXPORT solid_frame : public composite_component
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
