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
    /// \brief Sets which attribute (highlight or lowlight) is used to draw
    /// the frame.
    //* =====================================================================
    void set_highlight(bool highlight);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

MUNIN_EXPORT
std::shared_ptr<solid_frame> make_solid_frame();

}
