#pragma once

#include "munin/component.hpp"
#include "munin/export.hpp"
#include "munin/render_surface_capabilities.hpp"

#include <boost/signals2/signal.hpp>
#include <nlohmann/json.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/screen.hpp>
#include <terminalpp/terminal.hpp>

#include <any>
#include <memory>
#include <vector>


namespace munin {

//* =========================================================================
/// \brief An object that represents a top-level window.
//* =========================================================================
class MUNIN_EXPORT window
{
public:
    //* =====================================================================
    /// \brief Constructor
    /// \param terminal A terminal on which this window paints canvases.
    /// \param content A component that this window displays.  May not be
    ///        null.
    //* =====================================================================
    window(terminalpp::terminal &terminal, std::shared_ptr<component> content);

    //* =====================================================================
    /// \brief Constructor
    /// \param terminal A terminal on which this window paints canvases.
    /// \param content A component that this window displays.  May not be
    ///        null.
    /// \param capabilities the capabilities of the render surface.
    //* =====================================================================
    window(
        terminalpp::terminal &terminal,
        std::shared_ptr<component> content,
        render_surface_capabilities const &capabilities);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~window();

    //* =====================================================================
    /// \brief Send an event to the window.  This will be passed straight to
    /// the content.
    //* =====================================================================
    void event(std::any const &ev);

    //* =====================================================================
    /// \brief Writes a string to the terminal that represents the changes
    /// on the canvas since it was last painted.
    //* =====================================================================
    void repaint(terminalpp::canvas &cvs);

    //* =====================================================================
    /// \brief Returns a JSON representation of the current state of the
    /// window and its content.
    //* =====================================================================
    [[nodiscard]] nlohmann::json to_json() const;

    //* =====================================================================
    /// \fn on_repaint_request
    /// \brief Connect to this signal in order to receive notifications that
    /// the content of the window has been changed and required repainting.
    //* =====================================================================
    boost::signals2::signal<void()> on_repaint_request;  // NOLINT

private:
    std::shared_ptr<component> content_;
    std::vector<terminalpp::rectangle> repaint_regions_;
    terminalpp::screen screen_;
    render_surface_capabilities const &capabilities_;
};

}  // namespace munin
