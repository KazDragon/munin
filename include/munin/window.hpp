#pragma once

#include "munin/export.hpp"
#include <terminalpp/extent.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>

namespace munin {

class component;
class context;

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
    //* =====================================================================
    explicit window(std::shared_ptr<component> content);
    
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~window();
    
    //* =====================================================================
    /// \brief Sets the size of the window and the content therein.
    //* =====================================================================
    void set_size(terminalpp::extent size);

    //* =====================================================================
    /// \brief Send an event to the window.  This will be passed straight to
    /// the content.
    //* =====================================================================
    void event(boost::any const &ev);

    //* =====================================================================
    /// \brief Paints the content onto the given context.
    //* =====================================================================
    void repaint(context &cvs);

    //* =====================================================================
    /// \fn on_repaint_request
    /// \brief Connect to this signal in order to receive notifications that
    /// the content of the window has been changed and required repainting.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_repaint_request;
    
private :
    class impl;
    std::shared_ptr<impl> pimpl_;
};

}
