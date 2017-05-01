#pragma once

#include "munin/export.hpp"
#include <terminalpp/extent.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/signals2/signal.hpp>
#include <string>

namespace terminalpp {
    class terminal;
}

namespace munin {

class component;

//* =========================================================================
/// \brief An object that represents a top-level window.
//* =========================================================================
class MUNIN_EXPORT window
{
public :
    //* =====================================================================
    /// \brief Constructor
    /// \param content A component that this window displays.  May not be
    ///        null.
    /// \param terminal a terminal that is used to display this window.
    /// \param strand A boost::asio::strand in which all asynchronous calls
    /// will be run.
    //* =====================================================================
    window(
        std::shared_ptr<component> const &content,
        terminalpp::terminal             &terminal,
        boost::asio::io_service::strand  &strand);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~window();

    //* =====================================================================
    /// \brief Sets the size of the window.  Following this, the contents
    /// of the window are re-laid out.
    //* =====================================================================
    void set_size(terminalpp::extent size);

    //* =====================================================================
    /// \brief Sets the title of the window.
    //* =====================================================================
    void set_title(std::string const &title);

    //* =====================================================================
    /// \brief Enables mouse tracking for the window.
    //* =====================================================================
    void enable_mouse_tracking();

    //* =====================================================================
    /// \brief Sends data that has been received from the client.  This is
    /// expected to be in ANSI format, and will be converted to events that
    /// are passed down into the focussed component.
    //* =====================================================================
    void data(std::string const &data);

    //* =====================================================================
    /// \fn on_repaint
    /// \param paint_data A sequence of characters that contain the ANSI-
    /// compliant bytes necessary to repaint the window.
    /// \brief Connect to this signal in order to receive notification about
    /// when the window has repainted and the data for how to repaint it.
    //* =====================================================================
    boost::signals2::signal
    <
        void (std::string const &paint_data)
    > on_repaint;

private :
    class impl;
    std::shared_ptr<impl> pimpl_;
};

}
