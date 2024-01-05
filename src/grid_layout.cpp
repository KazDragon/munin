#include "munin/grid_layout.hpp"
#include "munin/component.hpp"
#include "munin/detail/json_adaptors.hpp"
#include <numeric>

namespace munin {

namespace {

// ==========================================================================
// INCREMENT_OF_DIMENSION
// ==========================================================================
terminalpp::point increment_of_dimension(
    terminalpp::point current_dimension, terminalpp::extent const &dimensions)
{
  ++current_dimension.x_;

  if ((current_dimension.x_ % dimensions.width_) == 0)
  {
    current_dimension.x_ = 0;
    ++current_dimension.y_;
  }

  if ((current_dimension.y_ % dimensions.height_) == 0)
  {
    current_dimension.y_ = 0;
  }

  return current_dimension;
}

}  // namespace

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
grid_layout::grid_layout(terminalpp::extent dimensions)
  : dimensions_(dimensions)
{
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent grid_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any> const &hints) const
{
  auto max_preferred_sizes = std::accumulate(
      components.begin(),
      components.end(),
      terminalpp::extent{},
      [this](auto preferred_size, auto const &component)
      {
        auto const &component_preferred_size = component->get_preferred_size();
        preferred_size.width_ =
            std::max(component_preferred_size.width_, preferred_size.width_);
        preferred_size.height_ =
            std::max(component_preferred_size.height_, preferred_size.height_);

        return preferred_size;
      });

  return {
      max_preferred_sizes.width_ * dimensions_.width_,
      max_preferred_sizes.height_ * dimensions_.height_};
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void grid_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any> const &hints,
    terminalpp::extent size) const
{
  terminalpp::extent const component_size = {
      size.width_ / dimensions_.width_, size.height_ / dimensions_.height_};

  terminalpp::extent const total_excess = {
      size.width_ % dimensions_.width_, size.height_ % dimensions_.height_};

  terminalpp::point current_dimension;

  for (auto const &component : components)
  {
    auto current_component_position = terminalpp::point{
        component_size.width_ * current_dimension.x_,
        component_size.height_ * current_dimension.y_};

    current_component_position.x_ +=
        std::min(current_dimension.x_, total_excess.width_);
    current_component_position.y_ +=
        std::min(current_dimension.y_, total_excess.height_);

    auto current_component_size =
        terminalpp::extent{component_size.width_, component_size.height_};

    if (current_dimension.x_ < total_excess.width_)
    {
      ++current_component_size.width_;
    }

    if (current_dimension.y_ < total_excess.height_)
    {
      ++current_component_size.height_;
    }

    component->set_position(current_component_position);
    component->set_size(current_component_size);

    current_dimension = increment_of_dimension(current_dimension, dimensions_);
  }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json grid_layout::do_to_json() const
{
  return {
      {"type", "grid_layout"}, {"dimensions", detail::to_json(dimensions_)}};
}

// ==========================================================================
// MAKE_GRID_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_grid_layout(terminalpp::extent size)
{
  return std::unique_ptr<layout>(new grid_layout(size));
}

}  // namespace munin
