#pragma once

#include "munin/composite_component.hpp"
#include "munin/frame.hpp"

namespace munin {

BOOST_STATIC_CONSTANT(std::string,
    FOCUSSED_BORDER_PEN = "Focussed.Border.Pen");
BOOST_STATIC_CONSTANT(std::string,
    UNFOCUSSED_BORDER_PEN = "Unfocussed.Border.Pen");

//* =========================================================================
/// \brief A class that models a multi-line text control with a frame
/// bordering it.
//* =========================================================================
class MUNIN_EXPORT framed_component
  : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    framed_component(
        std::shared_ptr<frame>     const &border
      , std::shared_ptr<component> const &interior);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~framed_component();

protected :
    //* =====================================================================
    /// \brief Called by set_attribute().  Derived classes must override this
    /// function in order to set an attribute in a custom manner.
    //* =====================================================================
    virtual void do_set_attribute(
        std::string const &name, boost::any const &attr);

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created framed component
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<component> make_framed_component(
    std::shared_ptr<frame>     const &border,
    std::shared_ptr<component> const &interior);

}
