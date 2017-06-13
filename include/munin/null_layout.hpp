#pragma once

#include "munin/layout.hpp"

namespace munin {

//* =========================================================================
/// \brief A class whose layout operations are no-ops.  It never lays out
/// any component, and its preferred size is always 0.
//* =========================================================================
class MUNIN_EXPORT null_layout
    : public layout
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints,
        terminalpp::extent                           size) const override;
};

//* =========================================================================
/// \brief Returns a newly created horizontal strip layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_null_layout();

}
