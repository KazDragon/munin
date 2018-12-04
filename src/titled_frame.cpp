#include "munin/titled_frame.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/compass_layout.hpp"
#include "munin/image.hpp"
#include "munin/view.hpp"

namespace munin {

struct titled_frame::impl
{
    impl(titled_frame &self)
      : self(self)
    {
    }
    
    titled_frame &self;
    terminalpp::string title_text;
    std::shared_ptr<image> title;
    terminalpp::attribute lowlight_attribute;
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold};
    terminalpp::attribute *current_attribute = &lowlight_attribute;

    void evaluate_focus(std::shared_ptr<component> const &associated_component)
    {
        auto *old_attribute = current_attribute;
        
        current_attribute = associated_component->has_focus()
                          ? &highlight_attribute
                          : &lowlight_attribute;
                          
        if (old_attribute != current_attribute)
        {
            redraw_frame();
        }
    }

    void redraw_frame()
    {
        auto size = self.get_size();
        auto northwest_beam_region = size.width > 4
          ? rectangle{{0, 0}, {2, 1}}
          : rectangle{{0, 0}, {size.width, 1}};
        
        auto skipped_section_width = 3 + title->get_size().width + 1;
        auto northeast_beam_region = rectangle{
            {skipped_section_width, 0},
            {size.width - (skipped_section_width), 1}};
        
        auto south_beam_region = size.height > 1
          ? rectangle{{0, size.height - 1}, {size.width, 1}}
          : rectangle{};
          
        auto west_beam_region  = size.height > 2
          ? rectangle{{0, 1}, {1, size.height - 2}}
          : rectangle{};
          
        auto east_beam_region  = size.height > 2
          ? rectangle{{size.width - 1, 1}, {1, size.height - 2}}
          : rectangle{};

        self.on_redraw({
            northwest_beam_region, northeast_beam_region,
            south_beam_region, west_beam_region, east_beam_region
        });
    }
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
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void titled_frame::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    pimpl_->highlight_attribute = highlight_attribute;
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void titled_frame::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    pimpl_->lowlight_attribute = lowlight_attribute;
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void titled_frame::highlight_on_focus(
    std::shared_ptr<component> const &associated_component)
{
    auto const evaluate_focus = 
        [this, wp = std::weak_ptr<component>(associated_component)]
        {
            std::shared_ptr<component> associated_component(wp.lock());

            if (associated_component)
            {
                pimpl_->evaluate_focus(associated_component);
            }
        };

    associated_component->on_focus_set.connect(evaluate_focus);
    associated_component->on_focus_lost.connect(evaluate_focus);
    evaluate_focus();
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
