#pragma once

#include "munin/basic_component.hpp"

namespace munin {

//* =========================================================================
/// A component that represents a text area.
//* =========================================================================
class MUNIN_EXPORT text_area : public basic_component
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed canvas.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(
        render_surface &surface,
        terminalpp::rectangle const &region) const override;
};

//* =========================================================================
/// \brief Returns a newly created empty text area.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<text_area> make_text_area();

}