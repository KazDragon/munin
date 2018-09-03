#pragma once

#include "munin/composite_component.hpp"
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
class MUNIN_EXPORT titled_frame : public composite_component
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
