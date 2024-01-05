#include "munin/brush.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <utility>


using namespace terminalpp::literals;  // NOLINT

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush() : brush(" "_ts)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush(terminalpp::string pattern)
  : brush(std::vector<terminalpp::string>{std::move(pattern)})
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush(std::vector<terminalpp::string> pattern)
  : pattern_(std::move(pattern))
{
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern()
{
  set_pattern(" ");
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern(terminalpp::string const &pattern)
{
  set_pattern(std::vector<terminalpp::string>{pattern});
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern(std::vector<terminalpp::string> const &pattern)
{
  pattern_ = pattern;
  on_preferred_size_changed();
  on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_CAN_RECEIVE_FOCUS
// ==========================================================================
bool brush::do_can_receive_focus() const
{
  return false;
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent brush::do_get_preferred_size() const
{
  auto const &size = [](auto const &val) { return val.size(); };

  using boost::adaptors::transformed;

  return pattern_.empty() ? terminalpp::extent(1, 1)
                          : terminalpp::extent(
                              terminalpp::coordinate_type(*boost::max_element(
                                  pattern_ | transformed(size))),
                              terminalpp::coordinate_type(pattern_.size()));
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void brush::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
  terminalpp::for_each_in_region(
      surface,
      region,
      [this](
          terminalpp::element &elem,
          terminalpp::coordinate_type column,  // NOLINT
          terminalpp::coordinate_type row)
      {
        auto const fill_row = row % pattern_.size();
        auto const fill_column = column % pattern_[fill_row].size();

        elem = pattern_[fill_row][fill_column];
      });
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json brush::do_to_json() const
{
  nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "brush" }
    ])"_json;

  auto json = basic_component::do_to_json().patch(patch);

  json["pattern"]["size"] = pattern_.size();

  for (size_t index = 0; index < pattern_.size(); ++index)
  {
    json["pattern"]["content"][index] = terminalpp::to_string(pattern_[index]);
  }

  return json;
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush()
{
  return std::make_shared<brush>();
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush(terminalpp::string pattern)
{
  return std::make_shared<brush>(std::move(pattern));
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush(std::vector<terminalpp::string> pattern)
{
  return std::make_shared<brush>(std::move(pattern));
}

}  // namespace munin
