#pragma once

#include "munin/composite_component.hpp"
#include <boost/signal.hpp>

namespace terminalpp {
    class string;
}

namespace munin {

//* =========================================================================
/// \brief A class that models a push-button.
//* =========================================================================
class MUNIN_EXPORT button
  : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    button(terminalpp::string const &caption);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~button();

    //* =====================================================================
    /// \fn on_click
    /// \brief Connect to this signal to receive click events due to the
    /// button either being clicked, or having focus and receiving an
    /// enter or space keypress.
    //* =====================================================================
    boost::signal<void ()> on_click;

protected :
    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created button
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<button> make_button(terminalpp::string const &caption);

}
