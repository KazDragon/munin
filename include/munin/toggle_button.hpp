#pragma once
#include "munin/composite_component.hpp"

namespace munin {
    
//* =========================================================================
/// \brief A class that implements a clickable button
//* =========================================================================
class MUNIN_EXPORT toggle_button : public munin::composite_component
{
public :  
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit toggle_button(bool checked = false);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Creates a new button
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<toggle_button> make_toggle_button(bool checked = false);

}
