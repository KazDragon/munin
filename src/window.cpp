#include "munin/window.hpp"

#include "munin/mouse_event.hpp"
#include "munin/render_surface.hpp"

#include <terminalpp/behaviour.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/terminal.hpp>

namespace munin {

namespace {

class inspect_mouse_capabilities
{
public:
    explicit inspect_mouse_capabilities(event_context &ctx) : context_(ctx)
    {
    }

    void operator()(
        terminalpp::behaviour const &beh,
        terminalpp::terminal_state &state,
        terminalpp::terminal::write_function const &write_fn) const
    {
        context_.set_click_synthesis_supported(
            beh.supports_basic_mouse_tracking
            && beh.supports_sgr_mouse_encoding);
    }

private:
    event_context &context_;
};

auto is_left_button_down(terminalpp::mouse::event const &ev) -> bool
{
    return ev.action_ == terminalpp::mouse::event_type::left_button_down
        && !ev.is_motion_ && !ev.is_release_;
}

auto is_button_up(terminalpp::mouse::event const &ev) -> bool
{
    return ev.action_ == terminalpp::mouse::event_type::button_up;
}

auto is_inside(terminalpp::point const &point, terminalpp::extent const &size)
    -> bool
{
    return point.x_ >= 0 && point.x_ < size.width_ && point.y_ >= 0
        && point.y_ < size.height_;
}

auto derived_mouse_event(
    mouse_event_type action, terminalpp::mouse::event const &ev)
    -> munin::mouse_event
{
    return munin::mouse_event{
        .action_ = action,
        .position_ = ev.position_,
        .button_ = ev.button_,
        .modifiers_ = ev.modifiers_};
}

auto translate(
    terminalpp::mouse::event const &ev, terminalpp::point const &origin)
    -> terminalpp::mouse::event
{
    return terminalpp::mouse::event{
        .action_ = ev.action_,
        .position_ = ev.position_ - origin,
        .button_ = ev.button_,
        .button_code_ = ev.button_code_,
        .modifiers_ = ev.modifiers_,
        .is_motion_ = ev.is_motion_,
        .is_release_ = ev.is_release_};
}

auto translate(munin::mouse_event const &ev, terminalpp::point const &origin)
    -> munin::mouse_event
{
    return munin::mouse_event{
        .action_ = ev.action_,
        .position_ = ev.position_ - origin,
        .button_ = ev.button_,
        .modifiers_ = ev.modifiers_};
}

}  // namespace

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
    terminal << inspect_mouse_capabilities{event_context_};

    auto const &request_repaint = [this](auto const &regions) {
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
void window::event(std::any const &ev)
{
    if (auto const *mouse_ev = std::any_cast<terminalpp::mouse::event>(&ev);
        mouse_ev != nullptr)
    {
        if (is_left_button_down(*mouse_ev))
        {
            event_context_.reset_mouse_dispatch_state();
            has_mouse_capture_ = true;
            event_context_.capture_mouse(content_, {});
            content_->event(ev, event_context_);
            content_->event(
                derived_mouse_event(mouse_event_type::button_down, *mouse_ev),
                event_context_);
            return;
        }

        if (has_mouse_capture_ && is_button_up(*mouse_ev))
        {
            auto captured_component = event_context_.captured_component();
            auto const captured_origin = event_context_.captured_origin();
            auto const local_mouse_event =
                translate(*mouse_ev, captured_origin);
            auto const local_button_up = translate(
                derived_mouse_event(mouse_event_type::button_up, *mouse_ev),
                captured_origin);

            if (captured_component)
            {
                captured_component->event(local_mouse_event, event_context_);
                captured_component->event(local_button_up, event_context_);
            }
            else
            {
                content_->event(ev, event_context_);
                content_->event(
                    derived_mouse_event(mouse_event_type::button_up, *mouse_ev),
                    event_context_);
            }

            if (event_context_.has_click_interest() && captured_component
                && is_inside(
                    local_mouse_event.position_,
                    captured_component->get_size()))
            {
                captured_component->event(
                    translate(
                        derived_mouse_event(mouse_event_type::click, *mouse_ev),
                        captured_origin),
                    event_context_);
            }

            event_context_.reset_mouse_dispatch_state();
            has_mouse_capture_ = false;
            return;
        }

        if (has_mouse_capture_)
        {
            auto captured_component = event_context_.captured_component();
            auto const captured_origin = event_context_.captured_origin();
            auto const local_mouse_event =
                translate(*mouse_ev, captured_origin);

            if (captured_component)
            {
                captured_component->event(local_mouse_event, event_context_);

                auto const is_now_inside = is_inside(
                    local_mouse_event.position_,
                    captured_component->get_size());

                if (is_now_inside != event_context_.captured_mouse_inside())
                {
                    captured_component->event(
                        translate(
                            derived_mouse_event(
                                is_now_inside ? mouse_event_type::in
                                              : mouse_event_type::out,
                                *mouse_ev),
                            captured_origin),
                        event_context_);
                    event_context_.set_captured_mouse_inside(is_now_inside);
                }
            }
            else
            {
                content_->event(ev, event_context_);
            }

            return;
        }
    }

    content_->event(ev, event_context_);
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
    return {
        {"type",    "window"           },
        {"content", content_->to_json()}
    };
}

}  // namespace munin
