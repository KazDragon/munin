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
    /// \brief Destructor
    //* =====================================================================
    ~toggle_button() override;

    //* =====================================================================
    /// \brief Sets the toggle state of the button.
    //* =====================================================================
    void set_toggle_state(bool checked);
    
    //* =====================================================================
    /// \fn on_state_changed
    /// An event that fires when the toggle state of the button changes.
    //* =====================================================================
    boost::signals2::signal<void (bool)> on_state_changed;

protected :
    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;

private :
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Creates a new button
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<toggle_button> make_toggle_button(bool checked = false);

}
