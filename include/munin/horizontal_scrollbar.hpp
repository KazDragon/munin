#include "munin/basic_component.hpp"

namespace munin {

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
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(
        render_surface &surface,
        terminalpp::rectangle const &region) const override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

MUNIN_EXPORT
std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar();

}