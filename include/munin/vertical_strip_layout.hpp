#pragma once

#include "munin/layout.hpp"
#include <boost/shared_ptr.hpp>

namespace munin {


//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// vertical strips.  Each component is given all the available height,
/// with the sum of the preferred widths of all the components.
///
/// \verbatim
/// +----------+
/// |+-+-----+ |
/// || |     | |
/// || |     | |
/// || |     | |
/// || |     | |
/// |+-+-----+ |
/// +----------+
/// \endverbatim
//* =========================================================================
class MUNIN_EXPORT vertical_strip_layout final
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

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;
};

//* =========================================================================
/// \brief Returns a newly created vertical strip layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_vertical_strip_layout();

}
