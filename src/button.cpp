#include "munin/button.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/image.hpp"
#include "munin/solid_frame.hpp"
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>
#include <utility>

namespace munin {

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
void button::do_event(boost::any const &ev)
{
  if (auto const *mouse_event = boost::any_cast<terminalpp::mouse::event>(&ev);
      mouse_event != nullptr)
  {
    if (mouse_event->action_ == terminalpp::mouse::event_type::left_button_down)
    {
      on_click();
    }
  }
  else if (auto const *vk = boost::any_cast<terminalpp::virtual_key>(&ev);
           vk != nullptr)
  {
    if (vk->key == terminalpp::vk::enter || vk->key == terminalpp::vk::space)
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

  return composite_component::do_to_json().patch(patch);
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<button> make_button(terminalpp::string const &text)
{
  return std::make_shared<button>(text);
}

}  // namespace munin
