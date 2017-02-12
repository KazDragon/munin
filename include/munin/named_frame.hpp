#pragma once

#include "munin/basic_frame.hpp"
#include <terminalpp/string.hpp>
#include <string>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief An object that represents a frame that has a title on its
/// northern edge.
//* =========================================================================
class MUNIN_EXPORT named_frame : public basic_frame
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    named_frame();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~named_frame();

    //* =====================================================================
    /// \brief Sets the name displayed on the top border of the frame.
    //* =====================================================================
    void set_name(terminalpp::string const &name);

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
/// \brief Returns a newly created named frame
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<named_frame> make_named_frame(terminalpp::string const &name);

}
