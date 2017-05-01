#pragma once

#ifndef MUNIN_STATUS_BAR_HPP_
#define MUNIN_STATUS_BAR_HPP_

#include "munin/composite_component.hpp"

namespace terminalpp {
    class string;
}

namespace munin {

//* =========================================================================
/// \brief A class that models a status_bar.
//* =========================================================================
class MUNIN_EXPORT status_bar
  : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    status_bar();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~status_bar();

    //* =====================================================================
    /// \brief Sets the message shown on the status bar.
    //* =====================================================================
    void set_message(terminalpp::string const &message);

protected :
    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param ctx the context in which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    virtual void do_draw(
        context         &ctx
      , rectangle const &region);

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
