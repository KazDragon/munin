#pragma once

#include "munin/composite_component.hpp"

#include <terminalpp/attribute.hpp>

namespace munin {

//* =========================================================================
/// \brief A base class for frames that exposes the border widths
//* =========================================================================
class MUNIN_EXPORT frame : public composite_component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    frame();

    //* =====================================================================
    /// \brief Associates the frame with an inner component, whose focus
    /// it will track for highlighting.
    //* =====================================================================
    void highlight_on_focus(std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Sets the lowlight attribute.
    //* =====================================================================
    void set_lowlight_attribute(
        terminalpp::attribute const &lowlight_attribute);

    //* =====================================================================
    /// \brief Sets the highlight attribute.
    //* =====================================================================
    void set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute);

    //* =====================================================================
    /// \brief returns the height of the north border.
    //* =====================================================================
    [[nodiscard]] virtual terminalpp::coordinate_type north_border_height()
        const = 0;

    //* =====================================================================
    /// \brief returns the height of the south border.
    //* =====================================================================
    [[nodiscard]] virtual terminalpp::coordinate_type south_border_height()
        const = 0;

    //* =====================================================================
    /// \brief returns the width of the west border.
    //* =====================================================================
    [[nodiscard]] virtual terminalpp::coordinate_type west_border_width()
        const = 0;

    //* =====================================================================
    /// \brief returns the width of the east border.
    //* =====================================================================
    [[nodiscard]] virtual terminalpp::coordinate_type east_border_width()
        const = 0;

protected:
    //* =====================================================================
    /// \brief Override this to describe how to highlight the frame on
    /// focus.  For example, if there are other sub-components that will
    /// need to listen to the focus of the inner component.
    //* =====================================================================
    virtual void do_highlight_on_focus(
        std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Override this to set the lowlight attribute in a custom
    /// manner.  For example, if there are other sub-components that will
    /// use the same attribute.
    //* =====================================================================
    virtual void do_set_lowlight_attribute(
        terminalpp::attribute const &lowlight_attribute);

    //* =====================================================================
    /// \brief Override this to set the highlight attribute in a custom
    /// manner.  For example, if there are other sub-components that will
    /// use the same attribute.
    //* =====================================================================
    virtual void do_set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute);

    //* =====================================================================
    /// \brief Returns true if it is allowed for the component to receive
    /// focus, false otherwise.  This is used by the focus functions of
    /// basic_component.  By default, a component may receive focus at all
    /// times.  Override this function to specify different behaviour in your
    /// component.
    //* =====================================================================
    [[nodiscard]] bool do_can_receive_focus() const override;

    //* =====================================================================
    /// \brief Gets the current focus attribute (be it the highlight or
    /// the lowlight.)
    //* =====================================================================
    [[nodiscard]] terminalpp::attribute get_focus_attribute() const;

    //* =====================================================================
    /// \brief Called when the focus of the associated component has changed
    /// Derived classes must override this to provide appropriate redraw
    /// callbacks so that the frame can be redrawn with this new focus.
    //* =====================================================================
    virtual void do_inner_focus_changed() = 0;

    std::weak_ptr<component> inner_component_;
    terminalpp::attribute lowlight_attribute_;
    terminalpp::attribute highlight_attribute_;
};

}  // namespace munin
