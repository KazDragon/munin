#pragma once

#include "munin/composite_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a given element.
//* =========================================================================
class MUNIN_EXPORT framed_component : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    framed_component(
        std::shared_ptr<component> const &frame,
        std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~framed_component() override;
};

//* =========================================================================
/// \brief Returns a newly created filled box.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component);

}
