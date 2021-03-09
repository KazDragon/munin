#pragma once

#include "munin/export.hpp"
#include <munin/component.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/screen.hpp>
#include <terminalpp/terminal.hpp>
#include <nlohmann/json.hpp>
#include <boost/any.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>
#include <vector>

namespace munin {

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
    /// \brief Send an event to the window.  This will be passed straight to
    /// the content.
    //* =====================================================================
    void event(boost::any const &ev);

    //* =====================================================================
    /// \brief Returns a string that represents the change in state of the
    /// window since the last repaint.
    //* =====================================================================
    template <typename WriteContinuation>
    void repaint(
        terminalpp::canvas &cvs,
        terminalpp::terminal &term,
        WriteContinuation &&wc)
    {
        auto const canvas_size = cvs.size();
        
        std::vector<terminalpp::rectangle> repaint_regions;
        
        if (cvs.size() != content_->get_size())
        {
            content_->set_size(cvs.size());
            repaint_regions.clear();
            repaint_regions.push_back({{}, canvas_size});
        }
        else
        {
            repaint_regions.swap(repaint_regions_);
        }

        render_surface surface(cvs);
        for (auto const &region : repaint_regions)
        {
            content_->draw(surface, region);
        }

        screen_.draw(term, cvs, std::forward<WriteContinuation>(wc));
    }

    //* =====================================================================
    /// \brief Returns a JSON representation of the current state of the
    /// window and its content.
    //* =====================================================================
    nlohmann::json to_json() const;
    
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
    std::shared_ptr<component> content_;
    std::vector<terminalpp::rectangle> repaint_regions_;
    terminalpp::screen screen_;
};

}
