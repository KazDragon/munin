#include "munin/button.hpp"

#include "munin/event_context.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/image.hpp"
#include "munin/mouse_event.hpp"
#include "munin/solid_frame.hpp"

#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <algorithm>
#include <utility>
#include <cctype>

namespace munin {

namespace {

auto stripped(terminalpp::string const &text) -> std::string
{
    auto result = terminalpp::to_string(text);
    auto const is_not_space = [](unsigned char ch) {
        return !std::isspace(ch);
    };

    result.erase(result.begin(), std::ranges::find_if(result, is_not_space));
    result.erase(
        std::ranges::find_if(result.rbegin(), result.rend(), is_not_space)
            .base(),
        result.end());
    return result;
}

}  // namespace

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
button::button(terminalpp::string text)
{
    auto image = make_image(std::move(text));
    image->set_can_receive_focus(true);

    set_layout(make_grid_layout({1, 1}));
    add_component(make_framed_component(make_solid_frame(), image));

    on_click.connect([this] { set_focus(); });
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void button::do_event(std::any const &ev, event_context &ctx)
{
    if (auto const *mouse_event = std::any_cast<munin::mouse_event>(&ev);
        mouse_event != nullptr)
    {
        if (mouse_event->action_ == munin::mouse_event_type::button_down)
        {
            if (!ctx.register_click_interest())
            {
                on_click();
            }
        }
        else if (mouse_event->action_ == munin::mouse_event_type::click)
        {
            on_click();
        }
    }
    else if (auto const *vk = std::any_cast<terminalpp::virtual_key>(&ev);
             vk != nullptr)
    {
        if (vk->key == terminalpp::vk::enter
            || vk->key == terminalpp::vk::space)
        {
            on_click();
        }
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json button::do_to_json() const
{
    static auto const patch = R"([
        { "op": "replace", "path": "/type", "value": "button" }
    ])"_json;

    auto json = composite_component::do_to_json().patch(patch);
    json["name"] = stripped(json.value("name", ""));
    return json;
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<button> make_button(terminalpp::string const &text)
{
    return std::make_shared<button>(text);
}

}  // namespace munin
