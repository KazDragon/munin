#include "munin/scroll_pane.hpp"

#include "munin/scroll_frame.hpp"
#include "munin/viewport.hpp"

#include <memory>

namespace munin {

// ==========================================================================
// SCROLL_PANE::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct scroll_pane::impl
{
    // ======================================================================
    // UPDATE_ANCHOR_BOUNDS
    // ======================================================================
    void update_anchor_bounds()
    {
        auto const anchor_bounds = viewport_->get_anchor_bounds();
        frame_->set_horizontal_slider_position(
            anchor_bounds.origin_.x_, anchor_bounds.size_.width_);
        frame_->set_vertical_slider_position(
            anchor_bounds.origin_.y_, anchor_bounds.size_.height_);
    }

    std::shared_ptr<scroll_frame> frame_;
    std::shared_ptr<viewport> viewport_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_pane::scroll_pane(std::shared_ptr<component> const &inner_component)
  : scroll_pane(
      make_scroll_frame(), make_viewport(inner_component), inner_component)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_pane::scroll_pane(
    std::shared_ptr<component> const &inner_component,
    std::unique_ptr<viewport::resize_strategy> viewport_resize_strategy)
  : scroll_pane(
      make_scroll_frame(),
      make_viewport(inner_component, std::move(viewport_resize_strategy)),
      inner_component)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_pane::scroll_pane(
    std::shared_ptr<component> const &inner_frame,
    std::shared_ptr<component> const &inner_viewport,  // NOLINT
    std::shared_ptr<component> const &inner_component)
  : framed_component(
      std::static_pointer_cast<frame>(inner_frame),
      std::static_pointer_cast<viewport>(inner_viewport)),
    pimpl_(std::make_unique<impl>())
{
    pimpl_->frame_ = std::static_pointer_cast<scroll_frame>(inner_frame);
    pimpl_->viewport_ = std::static_pointer_cast<viewport>(inner_viewport);

    pimpl_->viewport_->on_anchor_bounds_changed.connect(
        [this] { pimpl_->update_anchor_bounds(); });
    pimpl_->update_anchor_bounds();
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
scroll_pane::~scroll_pane() = default;

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void scroll_pane::set_highlight_attribute(terminalpp::attribute const &attr)
{
    pimpl_->frame_->set_highlight_attribute(attr);
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void scroll_pane::set_lowlight_attribute(terminalpp::attribute const &attr)
{
    pimpl_->frame_->set_lowlight_attribute(attr);
}

// ==========================================================================
// MAKE_SCROLL_PANE
// ==========================================================================
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<scroll_pane>(inner_component);
}

// ==========================================================================
// MAKE_SCROLL_PANE
// ==========================================================================
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component,
    std::unique_ptr<viewport::resize_strategy> strategy)
{
    return std::make_shared<scroll_pane>(inner_component, std::move(strategy));
}

}  // namespace munin
