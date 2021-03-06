#include "munin/window.hpp"
#include "munin/render_surface.hpp"
#include "munin/detail/json_adaptors.hpp"
#include <terminalpp/terminal.hpp>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
window::window(std::shared_ptr<component> content)
  : content_(std::move(content))
{
    auto const &request_repaint = 
        [this](auto const &regions)
        {
            bool first_request = repaint_regions_.empty();
            
            repaint_regions_.insert(
                repaint_regions_.end(), 
                regions.begin(),
                regions.end());

            if (first_request)
            {
                this->on_repaint_request();
            }
        };

    content_->on_redraw.connect(request_repaint);
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
    content_->event(ev);
}

// ==========================================================================
// TO_JSON
// ==========================================================================
nlohmann::json window::to_json() const
{
    return {
        { "type",    "window" },
        { "content", content_->to_json() }
    };
}

}
