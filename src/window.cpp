#include "munin/window.hpp"
#include "munin/component.hpp"
#include "munin/context.hpp"
#include "munin/detail/json_adaptors.hpp"

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
    terminalpp::extent size_;
    
    std::vector<rectangle> repaint_regions_;
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
// SET_SIZE
// ==========================================================================
void window::set_size(terminalpp::extent size)
{
    pimpl_->size_ = size;
    pimpl_->content_->set_size(size);
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
std::string window::repaint(context &ctx)
{
    std::vector<rectangle> repaint_regions;
    repaint_regions.swap(pimpl_->repaint_regions_);
    
    for (auto const &region : repaint_regions)
    {
        pimpl_->content_->draw(ctx, region);
    }
    
    return "";
}

// ==========================================================================
// TO_JSON
// ==========================================================================
nlohmann::json window::to_json() const
{
    return {
        { "type",    "window" },
        { "content", pimpl_->content_->to_json() },
        { "size",    detail::to_json(pimpl_->size_) }
    };
}

}
