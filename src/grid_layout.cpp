#include "munin/grid_layout.hpp"
#include "munin/component.hpp"
#include <numeric>

namespace munin {

namespace {
    
// ==========================================================================
// INCREMENT_OF_DIMENSION
// ==========================================================================
terminalpp::point increment_of_dimension(
    terminalpp::point current_dimension,
    terminalpp::extent const &dimensions)
{
    ++current_dimension.x;
    
    if ((current_dimension.x % dimensions.width) == 0)
    {
        current_dimension.x = 0;
        ++current_dimension.y;
    }
    
    if ((current_dimension.y % dimensions.height) == 0)
    {
        current_dimension.y = 0;
    }
    
    return current_dimension;
}

}

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
    std::vector<boost::any>                 const &hints) const
{
    auto max_preferred_sizes = std::accumulate(
        components.begin(),
        components.end(),
        terminalpp::extent{},
        [this](auto preferred_size, auto const &component)
    {
        auto const &component_preferred_size = component->get_preferred_size();
        preferred_size.width  = std::max(
            component_preferred_size.width, preferred_size.width);
        preferred_size.height = std::max(
            component_preferred_size.height, preferred_size.height);
            
        return preferred_size;
    });

    return {
        max_preferred_sizes.width  * dimensions_.width,
        max_preferred_sizes.height * dimensions_.height
    };
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void grid_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    terminalpp::extent const component_size = {
        size.width / dimensions_.width,
        size.height / dimensions_.height
    };

    terminalpp::extent const total_excess = {
        size.width % dimensions_.width,
        size.height % dimensions_.height
    };

    terminalpp::point current_dimension;

    for (auto &component : components)
    {
        auto current_component_position = terminalpp::point{        
            component_size.width * current_dimension.x,
            component_size.height * current_dimension.y
        };
        
        current_component_position.x += 
            std::min(current_dimension.x, total_excess.width);
        current_component_position.y += 
            std::min(current_dimension.y, total_excess.height);

        auto current_component_size = terminalpp::extent{
            component_size.width,
            component_size.height
        };
        
        if (current_dimension.x < total_excess.width)
        {
            ++current_component_size.width;
        }

        if (current_dimension.y < total_excess.height)
        {
            ++current_component_size.height;
        }

        component->set_position(current_component_position);
        component->set_size(current_component_size);
        
        current_dimension = 
            increment_of_dimension(current_dimension, dimensions_);
    }
}

// ==========================================================================
// MAKE_HORIZONTAL_STRIP_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_grid_layout(terminalpp::extent size)
{
    return std::unique_ptr<layout>(new grid_layout(size));
}

}

