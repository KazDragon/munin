#include "munin/window.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/terminal.hpp>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
window::window(
    terminalpp::terminal &terminal, std::shared_ptr<component> content)
  : window(terminal, std::move(content), default_capabilities)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
window::window(
    terminalpp::terminal &terminal,
    std::shared_ptr<component> content,
    render_surface_capabilities const &capabilities)
  : content_(std::move(content)), screen_{terminal}, capabilities_(capabilities)
{
  auto const &request_repaint = [this](auto const &regions)
  {
    bool first_request = repaint_regions_.empty();

    repaint_regions_.insert(
        repaint_regions_.end(), regions.begin(), regions.end());

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
window::~window() = default;

// ==========================================================================
// EVENT
// ==========================================================================
void window::event(boost::any const &ev)
{
  content_->event(ev);
}

// ==========================================================================
// REPAINT
// ==========================================================================
void window::repaint(terminalpp::canvas &cvs)
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

  render_surface surface(cvs, capabilities_);
  for (auto const &region : repaint_regions)
  {
    content_->draw(surface, region);
  }

  screen_.draw(cvs);
}

// ==========================================================================
// TO_JSON
// ==========================================================================
nlohmann::json window::to_json() const
{
  return {{"type", "window"}, {"content", content_->to_json()}};
}

}  // namespace munin
