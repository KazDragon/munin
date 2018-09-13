#pragma once
#include "munin/composite_component.hpp"
#include <terminalpp/string.hpp>

namespace munin {
    
//* =========================================================================
/// \brief A class that implements a clickable button
//* =========================================================================
class MUNIN_EXPORT button : public munin::composite_component
{
public :  
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit button(terminalpp::string text);
    
    boost::signals2::signal<void ()> on_click;
    
protected :
    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;
};

//* =========================================================================
/// \brief Creates a new button
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<button> make_button(terminalpp::string const &text);

}

