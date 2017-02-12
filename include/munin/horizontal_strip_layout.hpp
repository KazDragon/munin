#pragma once

#include "munin/layout.hpp"
#include <boost/shared_ptr.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// horizontal strips.  Each component is given all the available width,
/// with the sum of the preferred height of all the components.
///
/// +----------+
/// |+-------+ |
/// ||       | |
/// |+-------+ |
/// ||       | |
/// ||       | |
/// |+-------+ |
/// +----------+
//* =========================================================================
class MUNIN_EXPORT horizontal_strip_layout
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
      , std::vector<boost::any>                 const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints
      , terminalpp::extent                             size) override;
};

//* =========================================================================
/// \brief Returns a newly created horizontal strip layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_horizontal_strip_layout();

}
