#include "munin/filled_box.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
filled_box::filled_box(terminalpp::element const &element)
  : filled_box([element](auto) { return element; })
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
filled_box::filled_box(std::function<fill_function_type> fill_function)
  : fill_function_(std::move(fill_function)), preferred_size_({1, 1})
{
}

// ==========================================================================
// DO_CAN_RECEIVE_FOCUS
// ==========================================================================
bool filled_box::do_can_receive_focus() const
{
  return false;
}

// ==========================================================================
// SET_PREFERRED_SIZE
// ==========================================================================
void filled_box::set_preferred_size(terminalpp::extent preferred_size)
{
  preferred_size_ = preferred_size;
  on_preferred_size_changed();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent filled_box::do_get_preferred_size() const
{
  return preferred_size_;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void filled_box::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
  auto const element = fill_function_(surface);

  terminalpp::for_each_in_region(
      surface,
      region,
      [&element](
          terminalpp::element &elem,
          terminalpp::coordinate_type column,  // NOLINT
          terminalpp::coordinate_type row) { elem = element; });
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json filled_box::do_to_json() const
{
  nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "filled_box" }
    ])"_json;

  return basic_component::do_to_json().patch(patch);
}

// ==========================================================================
// MAKE_FILLED_BOX
// ==========================================================================
std::shared_ptr<filled_box> make_fill(terminalpp::element const &fill)
{
  return std::make_shared<filled_box>(fill);
}

// ==========================================================================
// MAKE_FILLED_BOX
// ==========================================================================
std::shared_ptr<filled_box> make_fill(
    std::function<terminalpp::element(render_surface &)> fill_function)
{
  return std::make_shared<filled_box>(std::move(fill_function));
}

}  // namespace munin
