#include "munin/framed_component.hpp"

#include <terminalpp/mouse.hpp>

namespace munin {
namespace {

class framed_component_layout : public munin::layout
{
protected:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<std::any> const &hints) const override
    {
        auto outer_frame = std::static_pointer_cast<frame>(components[0]);
        auto inner_preferred_size = components[1]->get_preferred_size();

        return terminalpp::extent{
            inner_preferred_size.width_ + outer_frame->west_border_width()
                + outer_frame->east_border_width(),
            inner_preferred_size.height_ + outer_frame->north_border_height()
                + outer_frame->south_border_height()};
    }

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<std::any> const &hints,
        terminalpp::extent size) const override
    {
        auto const &outer_frame =
            std::static_pointer_cast<frame>(components[0]);
        outer_frame->set_position({0, 0});
        outer_frame->set_size(size);

        auto const north_border_height = outer_frame->north_border_height();
        auto const south_border_height = outer_frame->south_border_height();
        auto const west_border_width = outer_frame->west_border_width();
        auto const east_border_width = outer_frame->east_border_width();

        auto const &inner = components[1];
        inner->set_position(
            {std::min(size.width_, west_border_width),
             std::min(size.height_, north_border_height)});

        auto const border_height = north_border_height + south_border_height;
        auto const border_width = west_border_width + east_border_width;

        inner->set_size(
            {size.width_ < border_width ? size.width_
                                        : size.width_ - border_width,
             size.height_ < border_height ? size.height_
                                          : size.height_ - border_height});
    }

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] nlohmann::json do_to_json() const override
    {
        return {
            {"type", "framed_layout"}
        };
    }
};

std::unique_ptr<layout> make_framed_component_layout()
{
    return std::unique_ptr<layout>(new framed_component_layout);
}

}  // namespace

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
framed_component::framed_component(
    std::shared_ptr<frame> outer_frame,
    std::shared_ptr<component> inner_component)
  : frame_(std::move(outer_frame)), inner_component_(std::move(inner_component))
{
    add_component(frame_);
    add_component(inner_component_);
    set_layout(make_framed_component_layout());

    frame_->highlight_on_focus(inner_component_);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void framed_component::do_event(std::any const &ev)
{
    if (auto const *mouse_event = std::any_cast<terminalpp::mouse::event>(&ev);
        mouse_event)
    {
        auto inner_mouse_event = *mouse_event;

        // Mouse events *always* hit the inner component, and never the
        // frame, so it needs adjusting inward and sending onward instead
        // of being handled by the default event handler, who will likely
        // send the event to the frame instead.

        // Translate mouse co-ordinates to inner-component co-ordinates
        auto inner_position = inner_component_->get_position();
        auto inner_size = inner_component_->get_size();

        inner_mouse_event.position_ -= inner_position;

        // And clamp it to the bounds of the component itself.
        inner_mouse_event.position_.x_ = std::max(
            0, std::min(inner_size.width_ - 1, inner_mouse_event.position_.x_));
        inner_mouse_event.position_.y_ = std::max(
            0,
            std::min(inner_size.height_ - 1, inner_mouse_event.position_.y_));

        inner_component_->event(inner_mouse_event);
    }
    else
    {
        composite_component::do_event(ev);
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json framed_component::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "framed_component" }
    ])"_json;

    auto json = composite_component::do_to_json().patch(patch);
    json["frame"] = frame_->to_json();
    json["component"] = inner_component_->to_json();

    return json;
}

// ==========================================================================
// MAKE_FRAMED_COMPONENT
// ==========================================================================
std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<frame> outer_frame,
    std::shared_ptr<component> inner_component)
{
    return std::make_shared<framed_component>(
        std::move(outer_frame), std::move(inner_component));
}

}  // namespace munin
