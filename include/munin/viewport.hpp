#pragma once

#include "munin/basic_component.hpp"

namespace munin {

//* =========================================================================
/// A component that tracks a subset of another component and shows only a
/// section of it.
//* =========================================================================
class MUNIN_EXPORT viewport : public basic_component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    viewport(std::shared_ptr<component> tracked_component);
    
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~viewport() override;
    
private:
    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_size(terminalpp::extent const &size) ;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    terminalpp::point do_get_cursor_position() const override;

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

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created viewport.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<viewport> make_viewport(
    std::shared_ptr<component> tracked_component);

}
