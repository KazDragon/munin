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

    //* =====================================================================
    /// \brief Sets the toggle state of the button.
    //* =====================================================================
    void set_toggle_state(bool checked);
    
    //* =====================================================================
    /// \fn on_state_changed
    /// An event that fires when the toggle state of the button changes.
    //* =====================================================================
    boost::signals2::signal<void (bool)> on_state_changed;

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