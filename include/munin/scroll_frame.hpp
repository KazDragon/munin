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
    /// \brief Constructor
    //* =====================================================================
    scroll_frame();

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    ~scroll_frame() override;

    //* =====================================================================
    /// \brief Sets the horizontal slider position
    //* =====================================================================
    void set_horizontal_slider_position(
        terminalpp::coordinate_type x_position,
        terminalpp::coordinate_type width);

    //* =====================================================================
    /// \brief Sets the vertical slider position
    //* =====================================================================
    void set_vertical_slider_position(
        terminalpp::coordinate_type y_position,
        terminalpp::coordinate_type height);

    //* =====================================================================
    /// \brief returns the height of the north border.
    //* =====================================================================
    [[nodiscard]] terminalpp::coordinate_type north_border_height()
        const override;

    //* =====================================================================
    /// \brief returns the height of the south border.
    //* =====================================================================
    [[nodiscard]] terminalpp::coordinate_type south_border_height()
        const override;

    //* =====================================================================
    /// \brief returns the width of the west border.
    //* =====================================================================
    [[nodiscard]] terminalpp::coordinate_type west_border_width()
        const override;

    //* =====================================================================
    /// \brief returns the width of the east border.
    //* =====================================================================
    [[nodiscard]] terminalpp::coordinate_type east_border_width()
        const override;

protected:
    //* =====================================================================
    /// \brief Override this to describe how to highlight the frame on
    /// focus.  For example, if there are other sub-components that will
    /// need to listen to the focus of the inner component.
    //* =====================================================================
    void do_highlight_on_focus(
        std::shared_ptr<component> const &inner_component) override;

    //* =====================================================================
    /// \brief Override this to set the lowlight attribute in a custom
    /// manner.  For example, if there are other sub-components that will
    /// use the same attribute.
    //* =====================================================================
    void do_set_lowlight_attribute(
        terminalpp::attribute const &lowlight_attribute) override;

    //* =====================================================================
    /// \brief Override this to set the highlight attribute in a custom
    /// manner.  For example, if there are other sub-components that will
    /// use the same attribute.
    //* =====================================================================
    void do_set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute) override;

    //* =====================================================================
    /// \brief Called when the focus of the associated component has changed
    /// Derived classes must override this to provide appropriate redraw
    /// callbacks so that the frame can be redrawn with this new focus.
    //* =====================================================================
    void do_inner_focus_changed() override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly-created scroll frame.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<scroll_frame> make_scroll_frame();

}  // namespace munin