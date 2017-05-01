#pragma once

#include "munin/export.hpp"
#include <boost/asio/strand.hpp>
#include <memory>

namespace terminalpp {
    class canvas_view;
}

namespace munin {

//* =========================================================================
/// \brief A context onto which components can draw themselves.
//* =========================================================================
class MUNIN_EXPORT context
{
public :
    //* =====================================================================
    /// \brief Constructor.
    //* =====================================================================
    context(terminalpp::canvas_view &cvs, boost::asio::strand &strand);

    //* =====================================================================
    /// \brief Destructor.
    //* =====================================================================
    ~context();

    //* =====================================================================
    /// \brief Retrieve the canvas.
    //* =====================================================================
    terminalpp::canvas_view &get_canvas();

    //* =====================================================================
    /// \brief Retrieve the strand.
    //* =====================================================================
    boost::asio::strand &get_strand();

private :
    class impl;
    std::shared_ptr<impl> pimpl_;
};

}
