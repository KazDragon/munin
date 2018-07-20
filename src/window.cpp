#include "munin/window.hpp"
#include "munin/component.hpp"
#include "munin/render_surface.hpp"
#include "munin/detail/json_adaptors.hpp"
#include <terminalpp/screen.hpp>
#include <terminalpp/terminal.hpp>

namespace munin {

// ==========================================================================
// WINDOW IMPLEMENTATION STRUCTURE
// ==========================================================================
struct window::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(window &self)
      : self_(self)
    {
    }
    
    // ======================================================================
    // REQUEST_REPAINT
    // ======================================================================
    void request_repaint(std::vector<rectangle> const &regions)
    {
        bool first_request = repaint_regions_.empty();
        
        repaint_regions_.insert(
            repaint_regions_.end(), 
            regions.begin(),
            regions.end());

        if (first_request)
        {
            self_.on_repaint_request();
        }
    }
    
    window &self_;
    std::shared_ptr<component> content_;

    std::vector<rectangle> repaint_regions_;

    terminalpp::screen screen_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
window::window(std::shared_ptr<component> content)
  : pimpl_(std::make_shared<impl>(std::ref(*this)))
{
    pimpl_->content_ = std::move(content);
    
    pimpl_->content_->on_redraw.connect(
        [this](auto const &regions)
        {
            pimpl_->request_repaint(regions);
        });
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
window::~window()
{
}

// ==========================================================================
// EVENT
// ==========================================================================
void window::event(boost::any const &ev)
{
    pimpl_->content_->event(ev);
}

// ==========================================================================
// REPAINT
// ==========================================================================
std::string window::repaint(
    terminalpp::canvas &cvs, terminalpp::terminal &term)
{
    auto const canvas_size = cvs.size();
    
    std::vector<rectangle> repaint_regions;
    
    if (cvs.size() != pimpl_->content_->get_size())
    {
        pimpl_->content_->set_size(cvs.size());
        repaint_regions.clear();
        repaint_regions.push_back({{}, canvas_size});
    }
    else
    {
        repaint_regions.swap(pimpl_->repaint_regions_);
    }

    render_surface surface(cvs);
    for (auto const &region : repaint_regions)
    {
        pimpl_->content_->draw(surface, region);
    }

    return pimpl_->screen_.draw(term, cvs);
}

// ==========================================================================
// TO_JSON
// ==========================================================================
nlohmann::json window::to_json() const
{
    return {
        { "type",    "window" },
        { "content", pimpl_->content_->to_json() }
    };
}

}
