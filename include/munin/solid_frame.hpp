#pragma once

#include "munin/basic_frame.hpp"

namespace munin {

class MUNIN_EXPORT solid_frame : public basic_frame
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    solid_frame();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~solid_frame();

    //* =====================================================================
    /// \brief Sets whether the frame is 'closeable' or not.  That is,
    /// whether it has a close icon that can be clicked.
    //* =====================================================================
    void set_closeable(bool closeable);

    //* =====================================================================
    /// \fn on_close
    /// \brief A signal that is raised whenever the close icon is clicked.
    //* =====================================================================
    boost::signal<
        void ()
    > on_close;

protected :
    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created solid frame
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<frame> make_solid_frame();

}
