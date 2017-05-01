#pragma once

#include "munin/layout.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// horizontal strips.  Each component is given a roughly even amount of space.
///
///  +--------+
///  |        |
///  +--------+
///  |        |
///  +--------+
//* =========================================================================
class MUNIN_EXPORT horizontal_squeeze_layout
    : public layout
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints) const;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints
      , terminalpp::extent                             size);
};

//* =========================================================================
/// \brief Returns a newly created horizontal squeeze layout layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_horizontal_squeeze_layout();

}
