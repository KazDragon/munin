#pragma once

#include <boost/asio.hpp>
#include <munin/window.hpp>

#include <memory>

namespace munin {

//* =========================================================================
/// \brief A binding to Console++.
///
/// \par
/// This is only available if Munin is built with MUNIN_WITH_CONSOLEPP=True.
///
/// \par
/// To use, create a munin::window wrapped around your user interface,
/// and an instance of boost::asio::io_context.  Running the io_context
/// will handle events between the console and window.
/// \code
/// munin::window window{my_terminal, my_ui, my_caps};
/// boost::asio::io_context io_context;
/// munin::console_application app{window, io_context};
///
/// auto work_guard = boost::asio::make_work_guard(io_context_);
/// // Note: to terminate the application, cancel the work guard
/// // and stop the io_context.
/// io_context.run(); // Application is now running.
/// \endcode
///
//* =========================================================================
class MUNIN_EXPORT console_application  // NOLINT
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    console_application(
        terminalpp::behaviour const &beh,
        boost::asio::io_context &io_context,
        std::shared_ptr<component> content);

    //* =====================================================================
    /// \brief Access the underlying terminal
    //* =====================================================================
    terminalpp::terminal &terminal();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~console_application();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}  // namespace munin
