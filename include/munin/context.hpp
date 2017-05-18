#pragma once

#include "munin/export.hpp"
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
    context(terminalpp::canvas_view &cvs);

    //* =====================================================================
    /// \brief Destructor.
    //* =====================================================================
    ~context();

    //* =====================================================================
    /// \brief Retrieve the canvas.
    //* =====================================================================
    terminalpp::canvas_view &get_canvas();

private :
    class impl;
    std::shared_ptr<impl> pimpl_;
};

}
