#pragma once

#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/rectangle.hpp>
#include <boost/range/algorithm_ext/insert.hpp>
#include <vector>

template <typename Component>
auto redraw_component_on_surface(
    Component &component, munin::render_surface &surface)
{
  return [&surface, &component](auto const &regions)
  {
    for (auto const &region : regions)
    {
      component.draw(surface, region);
    }
  };
}

template <typename Container>
auto append_regions_to_container(Container &container)
{
  return [&container](auto const &regions)
  { boost::insert(container, container.end(), regions); };
}

void assert_equivalent_redraw_regions(
    std::vector<terminalpp::rectangle> const &lhs,
    std::vector<terminalpp::rectangle> const &rhs);