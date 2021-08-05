#include "munin/scroll_pane.hpp"
#include "munin/scroll_frame.hpp"
#include "munin/viewport.hpp"
#include <boost/make_unique.hpp>

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
      make_scroll_frame(), 
      make_viewport(inner_component),
      inner_component)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
scroll_pane::scroll_pane(
    std::shared_ptr<component> const &inner_frame,
    std::shared_ptr<component> const &inner_viewport,
    std::shared_ptr<component> const &inner_component)
  : framed_component(
      std::static_pointer_cast<frame>(inner_frame),
      std::static_pointer_cast<viewport>(inner_viewport)),
    pimpl_(boost::make_unique<impl>())
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
// MAKE_SCROLL_PANE
// ==========================================================================
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<scroll_pane>(inner_component);
}

}
