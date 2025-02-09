#include "munin/basic_component.hpp"

#include <terminalpp/element.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that displays a horizontal scrollbar that displays the
/// horizontal position of e.g. a viewport relative to its underlying
/// component.
//* =========================================================================
class MUNIN_EXPORT horizontal_scrollbar : public basic_component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    horizontal_scrollbar();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~horizontal_scrollbar() override;

    //* =====================================================================
    /// \brief Sets the slider position.
    ///
    /// x_position and width relate the the bounds of the viewport that this
    /// scrollbar is showing.  For example, if a viewport has size 20, and
    /// the underlying component is size 50, then the width of the slider
    /// would be (50 - 20) = 30, which corresponds to the number of possible
    /// places for the viewport to be based.  x_position thus relates to the
    /// where the viewport is based.
    ///
    /// If width is zero, then that indicates that the entire underlying
    /// component is in view, and therefore no slider is shown.
    //* =====================================================================
    void set_slider_position(
        terminalpp::coordinate_type x_position,
        terminalpp::coordinate_type width);

    //* =====================================================================
    /// \brief Associates the scrollbar with another component, whose focus
    /// it will track for highlighting.
    //* =====================================================================
    void highlight_on_focus(
        std::shared_ptr<component> const &associated_component);

    //* =====================================================================
    /// \brief Sets the lowlight attribute.  This is the attribute shown
    /// when there is no associated component, or the associated component
    /// does not have focus.
    //* =====================================================================
    void set_lowlight_attribute(
        terminalpp::attribute const &lowlight_attribute);

    //* =====================================================================
    /// \brief Sets the highlight attribute.  This is the attribute hsown
    /// when there an associated component has focus.
    //* =====================================================================
    void set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute);

    //* =====================================================================
    /// \brief Connect to this signal to receive notifications when a scroll
    /// left instruction was received (e.g. by clicking to the left of the
    /// slider)
    //* =====================================================================
    boost::signals2::signal<void()> on_scroll_left;

    //* =====================================================================
    /// \brief Connect to this signal to receive notifications when a scroll
    /// right instruction was received (e.g. by clicking to the right of the
    /// slider)
    //* =====================================================================
    boost::signals2::signal<void()> on_scroll_right;

protected:
    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(render_surface &surface, terminalpp::rectangle const &region)
        const override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

MUNIN_EXPORT
std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar();

}  // namespace munin