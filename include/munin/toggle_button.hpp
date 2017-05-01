#pragma once

#include "munin/composite_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that models a toggle-button.
//* =========================================================================
class MUNIN_EXPORT toggle_button
  : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    toggle_button(bool default_state);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~toggle_button();

    //* =====================================================================
    /// \brief Sets whether the button is currently toggled or not.
    //* =====================================================================
    void set_toggle(bool toggle);

    //* =====================================================================
    /// \brief Retrieves the current toggle state.
    //* =====================================================================
    bool get_toggle() const;

    //* =====================================================================
    /// \fn on_toggle
    /// \brief Connect to this signal to receive toggle events due to the
    /// button either being clicked, or having focus and receiving an
    /// enter or space keypress.
    //* =====================================================================
    boost::signal<void (bool)> on_toggle;

protected :
    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
