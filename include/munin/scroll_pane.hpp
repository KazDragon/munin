#pragma once

#include "munin/composite_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A container that constructs a scroll pane around an underlying
/// component.
//* =========================================================================
class MUNIN_EXPORT scroll_pane
    : public composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    scroll_pane(
        std::shared_ptr<component> const &underlying_component
      , bool                              top_border = true);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~scroll_pane();

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created scroll pane
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<component> make_scroll_pane(
    std::shared_ptr<component> const &underlying_component,
    bool                              top_border = true);

}
