#include "munin/compass_layout.hpp"
#include "munin/component.hpp"
#include <utility>

namespace munin {

namespace {

struct used_headings
{
    explicit used_headings(terminalpp::extent size = {})
      : size_(std::move(size))
    {
    }

    terminalpp::coordinate_type north = 0;
    terminalpp::coordinate_type south = 0;
    terminalpp::coordinate_type east  = 0;
    terminalpp::coordinate_type west  = 0;

    terminalpp::coordinate_type remaining_width() const
    {
        return size_.width - west - east;
    }

    terminalpp::coordinate_type remaining_height() const
    {
        return size_.height - north - south;
    }

private :
    terminalpp::extent size_;
};

// ==========================================================================
// LAYOUT_CENTRE_COMPONENT
// ==========================================================================
static void layout_centre_component(
    component &comp,
    used_headings const &headings)
{
    comp.set_position({headings.west, headings.north});
    comp.set_size({headings.remaining_width(), headings.remaining_height()});
}

// ==========================================================================
// LAYOUT_NORTH_COMPONENT
// ==========================================================================
static void layout_north_component(
    component &comp,
    used_headings &headings)
{
    auto const height = (std::min)(
        comp.get_preferred_size().height,
        headings.remaining_height());

    comp.set_position({headings.west, 0});
    comp.set_size({headings.remaining_width(), height});

    headings.north = height;
}

// ==========================================================================
// LAYOUT_SOUTH_COMPONENT
// ==========================================================================
static void layout_south_component(
    component &comp,
    used_headings &headings)
{
    auto const preferred_size   = comp.get_preferred_size();
    auto const remaining_height = headings.remaining_height();
    auto const remaining_width  = headings.remaining_width();
    auto const height = (std::min)(preferred_size.height, remaining_height);

    comp.set_position({
        headings.west,
        (std::max)(
            (headings.north + remaining_height) - preferred_size.height,
            0)
    });
    comp.set_size({remaining_width, height});

    headings.south = height;
}

// ==========================================================================
// LAYOUT_WEST_COMPONENT
// ==========================================================================
static void layout_west_component(
    component &comp,
    used_headings &headings)
{
    auto const width = (std::min)(
        comp.get_preferred_size().width,
        headings.remaining_width());

    comp.set_position({0, headings.north});
    comp.set_size({width, headings.remaining_height()});

    headings.west = width;
}

// ==========================================================================
// LAYOUT_EAST_COMPONENT
// ==========================================================================
static void layout_east_component(
    component &comp,
    used_headings &headings)
{
    auto const preferred_size   = comp.get_preferred_size();
    auto const remaining_height = headings.remaining_height();
    auto const remaining_width  = headings.remaining_width();
    auto const width = (std::min)(preferred_size.width, remaining_width);

    comp.set_position({
        (std::max)(
            (headings.west + remaining_width) - preferred_size.width,
            0),
        headings.north});
    comp.set_size({width, remaining_height});

    headings.east = width;
}

}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent compass_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // Keeps track of the "unused" area that a centre component would fit 
    // into. For example, with a north component of (3,1) and an east 
    // component of (1, 3), then there is an open area of (2, 2) remaining.
    // Therefore, a centre component of (3, 3) would only affect the preferred
    // size by an extra (1, 1).
    terminalpp::extent unused_size;

    // Keeps track of the preferred centre size.  This must be handled last
    // due to it requiring knowledge of the unused size.
    terminalpp::extent preferred_centre;

    // The value that accumulates the preferred size for this group of
    // components.
    terminalpp::extent preferred_size;

    for (auto index = 0u; index < components.size(); ++index)
    {
        auto &comp            = *components[index];
        auto const *hint_any  = boost::any_cast<heading>(&hints[index]);
        auto const hint       = hint_any ? *hint_any : heading::centre;
        auto const &comp_preferred_size = comp.get_preferred_size();

        switch (hint)
        {
            default :
                // Fall-through
            case heading::centre :
                preferred_centre.width = std::max(
                    preferred_centre.width,
                    comp_preferred_size.width);
                preferred_centre.height = std::max(
                    preferred_centre.height,
                    comp_preferred_size.height);
                break;

            case heading::north :
                // fall-through
            case heading::south :
                unused_size.width = std::max(
                    unused_size.width,
                    comp_preferred_size.width - preferred_size.width);
                    
                preferred_size.width = std::max(
                    preferred_size.width,
                    comp_preferred_size.width);
                preferred_size.height += comp_preferred_size.height;    
                break;
                
            case heading::west :
                // fall-through
            case heading::east :
                unused_size.height = std::max(
                    unused_size.height,
                    comp_preferred_size.height - preferred_size.height);
                    
                preferred_size.width += comp_preferred_size.width;    
                preferred_size.height = std::max(
                    preferred_size.height,
                    comp_preferred_size.height);
              break;
        }
    }

    preferred_centre.width = std::max(
        preferred_centre.width - unused_size.width,
        0);
    preferred_centre.height = std::max(
        preferred_centre.height - unused_size.height,
        0);
        
    preferred_size.width += preferred_centre.width;
    preferred_size.height += preferred_centre.height;
    
    return preferred_size;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void compass_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size) const
{
    std::vector<component*> centre_components;
    used_headings headings(size);

    for (auto index = 0u; index < components.size(); ++index)
    {
        auto &comp           = *components[index];
        auto const *hint_any = boost::any_cast<heading>(&hints[index]);
        auto const hint      = hint_any ? *hint_any : heading::centre;

        switch (hint)
        {
            default :
                // fall-through
            case heading::centre :
                centre_components.push_back(&comp);
                break;

            case heading::north :
                layout_north_component(comp, headings);
                break;

            case heading::south :
                layout_south_component(comp, headings);
                break;

            case heading::west :
                layout_west_component(comp, headings);
                break;

            case heading::east :
                layout_east_component(comp, headings);
                break;
        }
    }

    for (auto *comp : centre_components)
    {
        layout_centre_component(*comp, headings);
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json compass_layout::do_to_json() const
{
    return {
        { "type", "compass_layout" },
    };
}

// ==========================================================================
// MAKE_COMPASS_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_compass_layout()
{
    return std::unique_ptr<layout>(new compass_layout);
}

}

