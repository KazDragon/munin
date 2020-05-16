#include "munin/titled_frame.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/compass_layout.hpp"
#include "munin/image.hpp"
#include "munin/view.hpp"

namespace munin {

// ==========================================================================
// TITLED_FRAME::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct titled_frame::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(titled_frame &self)
      : self(self)
    {
    }
    
    // ======================================================================
    // REDRAW_FRAME
    // ======================================================================
    void redraw_frame()
    {
        auto size = self.get_size();
        auto northwest_beam_region = size.width > 4
          ? terminalpp::rectangle{{0, 0}, {2, 1}}
          : terminalpp::rectangle{{0, 0}, {size.width, 1}};
        
        auto skipped_section_width = 3 + title->get_size().width + 1;
        auto northeast_beam_region = terminalpp::rectangle{
            {skipped_section_width, 0},
            {size.width - (skipped_section_width), 1}};
        
        auto south_beam_region = size.height > 1
          ? terminalpp::rectangle{{0, size.height - 1}, {size.width, 1}}
          : terminalpp::rectangle{};
          
        auto west_beam_region  = size.height > 2
          ? terminalpp::rectangle{{0, 1}, {1, size.height - 2}}
          : terminalpp::rectangle{};
          
        auto east_beam_region  = size.height > 2
          ? terminalpp::rectangle{{size.width - 1, 1}, {1, size.height - 2}}
          : terminalpp::rectangle{};

        self.on_redraw({
            northwest_beam_region, northeast_beam_region,
            south_beam_region, west_beam_region, east_beam_region
        });
    }

    titled_frame &self;
    terminalpp::string title_text;
    std::shared_ptr<image> title;
    terminalpp::attribute current_attribute;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
titled_frame::titled_frame(terminalpp::string const &title)
  : pimpl_(std::make_shared<impl>(*this))
{
    pimpl_->title_text = title;
    pimpl_->title = make_image(pimpl_->title_text);
    
    auto &attr = pimpl_->current_attribute;
    
    auto title_piece = view(
        make_compass_layout(),
        make_fill(' '), compass_layout::heading::west,
        pimpl_->title, compass_layout::heading::centre,
        make_fill(' '), compass_layout::heading::east);
        
    auto title_banner = view(
        make_compass_layout(),
        title_piece, compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr), compass_layout::heading::centre);
        
    auto north_beam = view(
        make_compass_layout(),
        detail::make_top_left_corner_fill(attr), compass_layout::heading::west,
        view(
            make_compass_layout(),
            detail::make_horizontal_beam_fill(attr), compass_layout::heading::west,
            title_banner, compass_layout::heading::centre,
            detail::make_horizontal_beam_fill(attr), compass_layout::heading::east),
        detail::make_top_right_corner_fill(attr), compass_layout::heading::east);
    
    auto south_beam = view(
        make_compass_layout(),
        detail::make_bottom_left_corner_fill(attr), compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr), compass_layout::heading::centre,
        detail::make_bottom_right_corner_fill(attr), compass_layout::heading::east);
        
    auto west_beam = detail::make_vertical_beam_fill(attr);
    auto east_beam = detail::make_vertical_beam_fill(attr);
    
    set_layout(make_compass_layout());
    
    add_component(north_beam, compass_layout::heading::north);
    add_component(south_beam, compass_layout::heading::south);
    add_component(west_beam, compass_layout::heading::west);
    add_component(east_beam, compass_layout::heading::east);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
titled_frame::~titled_frame()
{
}

// ==========================================================================
// NORTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type titled_frame::north_border_height() const
{
    return 1;    
}

// ==========================================================================
// SOUTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type titled_frame::south_border_height() const
{
    return 1;    
}

// ==========================================================================
// WEST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type titled_frame::west_border_width() const
{
    return 1;    
}

// ==========================================================================
// EAST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type titled_frame::east_border_width() const
{
    return 1;    
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json titled_frame::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "titled_frame" }
    ])"_json;

    auto json = composite_component::do_to_json().patch(patch);
    json["title"] = to_string(pimpl_->title_text);
    
    return json;
}

// ==========================================================================
// DO_INNER_FOCUS_CHANGED
// ==========================================================================
void titled_frame::do_inner_focus_changed()
{
    pimpl_->current_attribute = get_focus_attribute();
    pimpl_->redraw_frame();
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<titled_frame> make_titled_frame(
    terminalpp::string const &title)
{
    return std::make_shared<titled_frame>(title);
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<titled_frame> make_titled_frame(
    terminalpp::string const &title,
    std::shared_ptr<component> const &associated_component)
{
    auto frame = make_titled_frame(title);
    frame->highlight_on_focus(associated_component);
    return frame;
}

}
