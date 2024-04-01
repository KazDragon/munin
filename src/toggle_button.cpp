#include "munin/toggle_button.hpp"

#include "munin/filled_box.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/solid_frame.hpp"

#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <memory>

namespace munin {

// ==========================================================================
// TOGGLE_BUTTON::IMPL
// ==========================================================================
struct toggle_button::impl
{
    std::shared_ptr<component> fill;
    bool toggle_state = false;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
toggle_button::toggle_button(bool checked) : pimpl_(std::make_unique<impl>())
{
    pimpl_->fill = make_fill([this](render_surface &) -> terminalpp::element {
        return pimpl_->toggle_state ? 'X' : ' ';
    });

    pimpl_->toggle_state = checked;

    set_layout(make_grid_layout({1, 1}));
    add_component(make_framed_component(make_solid_frame(), pimpl_->fill));
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
toggle_button::~toggle_button() = default;

// ==========================================================================
// SET_TOGGLE_STATE
// ==========================================================================
void toggle_button::set_toggle_state(bool checked)
{
    if (checked != pimpl_->toggle_state)
    {
        pimpl_->toggle_state = checked;
        on_state_changed(pimpl_->toggle_state);
        on_redraw({
            {{pimpl_->fill->get_position()}, {pimpl_->fill->get_size()}}
        });
    }
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void toggle_button::do_event(boost::any const &ev)
{
    if (auto const *mouse_event =
            boost::any_cast<terminalpp::mouse::event>(&ev);
        mouse_event != nullptr)
    {
        if (mouse_event->action_
            == terminalpp::mouse::event_type::left_button_down)
        {
            set_toggle_state(!pimpl_->toggle_state);
        }
    }
    else if (auto const *vk = boost::any_cast<terminalpp::virtual_key>(&ev);
             vk != nullptr)
    {
        if (vk->key == terminalpp::vk::enter
            || vk->key == terminalpp::vk::space)
        {
            set_toggle_state(!pimpl_->toggle_state);
        }
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json toggle_button::do_to_json() const
{
    static auto const patch = R"([
        { "op": "replace", "path": "/type", "value": "toggle_button" }
    ])"_json;

    auto json = composite_component::do_to_json().patch(patch);
    json["state"] = pimpl_->toggle_state;

    return json;
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<toggle_button> make_toggle_button(bool checked)
{
    return std::make_shared<toggle_button>(checked);
}

}  // namespace munin
