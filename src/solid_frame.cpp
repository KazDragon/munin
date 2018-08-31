#include "munin/solid_frame.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/compass_layout.hpp"
#include "munin/render_surface.hpp"
#include "munin/view.hpp"

namespace munin {

struct solid_frame::impl
{
    impl(solid_frame &self)
      : self(self)
    {
    }
    
    solid_frame &self;
    terminalpp::attribute lowlight_attribute;
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold};
    terminalpp::attribute *current_attribute = &lowlight_attribute;

    void redraw_frame()
    {
        auto size = self.get_size();
        
        if (size.width > 2 && size.height > 2)
        {
            // Here we individually pick out the frame edges and redraw them.
            auto north_beam_region = munin::rectangle{{0,0}, {size.width, 1}};
            auto south_beam_region = munin::rectangle{{0, size.height - 1}, {size.width, 1}};
            auto west_beam_region  = munin::rectangle{{0, 1}, {1, size.height - 2}};
            auto east_beam_region  = munin::rectangle{{size.width - 1, 1}, {1, size.height - 2}};
    
            self.on_redraw({
                north_beam_region, south_beam_region, west_beam_region, east_beam_region
            });
        }
        else
        {
            // But if only our border is showing, then the redraw region is
            // the complete frame area.
            self.on_redraw({
                {{0, 0}, size}
            });
        }
    }
    
    void evaluate_focus(std::shared_ptr<component> const &associated_component)
    {
        auto *old_attribute = current_attribute;
        
        current_attribute = associated_component->has_focus()
                          ? &highlight_attribute
                          : &lowlight_attribute;
                          
        if (current_attribute != old_attribute)
        {
            redraw_frame();
        }
    }
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
solid_frame::solid_frame()
  : pimpl_(std::make_shared<impl>(*this))
{
    auto &attr = pimpl_->current_attribute;
    
    auto north_beam = view(
        make_compass_layout(),
        detail::make_top_left_corner_fill(attr), compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr), compass_layout::heading::centre,
        detail::make_top_right_corner_fill(attr),  compass_layout::heading::east);
    
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
solid_frame::~solid_frame()
{
}

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void solid_frame::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    pimpl_->highlight_attribute = highlight_attribute;
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void solid_frame::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    pimpl_->lowlight_attribute = lowlight_attribute;
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void solid_frame::highlight_on_focus(
    std::shared_ptr<component> const &associated_component)
{
    auto evaluate_focus = 
        [this, wp = std::weak_ptr<component>(associated_component)]()
        {
            std::shared_ptr<component> comp(wp);
            
            if (comp)
            {
                pimpl_->evaluate_focus(comp);
            }
        };
        
    associated_component->on_focus_set.connect(evaluate_focus);
    associated_component->on_focus_lost.connect(evaluate_focus);
    evaluate_focus();
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json solid_frame::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "solid_frame" }
    ])"_json;

    return composite_component::do_to_json().patch(patch);
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<solid_frame> make_solid_frame()
{
    return std::make_shared<solid_frame>();
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<solid_frame> make_solid_frame(
    std::shared_ptr<component> const &associated_component)
{
    auto frame = make_solid_frame();
    frame->highlight_on_focus(associated_component);
    return frame;
}

}
