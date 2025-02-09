#pragma once

#include "munin/basic_component.hpp"

#include <terminalpp/string.hpp>

namespace munin {

class animator;

//* =========================================================================
/// \brief A class that models a marquee of scrolling text that can be used
/// to display transient status messages.
//* =========================================================================
class MUNIN_EXPORT status_bar : public basic_component,
                                public std::enable_shared_from_this<status_bar>
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit status_bar(animator &anim);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~status_bar() override;

    //* =====================================================================
    /// \brief Sets the visible message on the status bar.
    //* =====================================================================
    void set_message(terminalpp::string const &message);

private:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed surface.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(render_surface &surface, terminalpp::rectangle const &region)
        const override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Creates a new status bar
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<status_bar> make_status_bar(animator &anim);

}  // namespace munin
