#include "munin/solid_frame.hpp"
#include "munin/detail/unicode_glyphs.hpp"
#include "munin/compass_layout.hpp"
#include "munin/filled_box.hpp"
#include "munin/graphics.hpp"
#include "munin/view.hpp"

namespace munin {
namespace {

constexpr terminalpp::glyph const default_corner_glyph          = '+';
constexpr terminalpp::glyph const default_horizontal_beam_glyph = '-';
constexpr terminalpp::glyph const default_vertical_beam_glyph   = '|';
    
// ==========================================================================
// TOP_LEFT_CORNER_GLYPH
// ==========================================================================
terminalpp::glyph top_left_corner_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_top_left_corner
         : default_corner_glyph;
}

// ==========================================================================
// TOP_RIGHT_CORNER_GLYPH
// ==========================================================================
terminalpp::glyph top_right_corner_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_top_right_corner
         : default_corner_glyph;
}

// ==========================================================================
// BOTTOM_LEFT_CORNER_GLYPH
// ==========================================================================
terminalpp::glyph bottom_left_corner_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_bottom_left_corner
         : default_corner_glyph;
}

// ==========================================================================
// BOTTOM_RIGHT_CORNER_GLYPH
// ==========================================================================
terminalpp::glyph bottom_right_corner_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_bottom_right_corner
         : default_corner_glyph;
}

// ==========================================================================
// HORIZONTAL_BEAM_GLYPH
// ==========================================================================
terminalpp::glyph horizontal_beam_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_horizontal_beam
         : default_horizontal_beam_glyph;
}

// ==========================================================================
// VERTICAL_BEAM_GLYPH
// ==========================================================================
terminalpp::glyph vertical_beam_glyph()
{
    return get_graphics().supports_unicode()
         ? detail::double_lined_vertical_beam
         : default_vertical_beam_glyph;
}

}

struct solid_frame::impl
{
    terminalpp::attribute lowlight_attribute = {};
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold};

    std::shared_ptr<munin::filled_box> top_left = 
        munin::make_fill({top_left_corner_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> top_centre = 
        munin::make_fill({horizontal_beam_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> top_right = 
        munin::make_fill({top_right_corner_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> centre_left = 
        munin::make_fill({vertical_beam_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> centre_right = 
        munin::make_fill({vertical_beam_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_left = 
        munin::make_fill({bottom_left_corner_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_centre = 
        munin::make_fill({horizontal_beam_glyph(), lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_right = 
        munin::make_fill({bottom_right_corner_glyph(), lowlight_attribute});
    
    bool is_highlighted = false;
    
    void set_fills()
    {
        auto const &attr = is_highlighted ? highlight_attribute : lowlight_attribute;
        
        top_left->set_fill({top_left_corner_glyph(), attr});
        top_centre->set_fill({horizontal_beam_glyph(), attr});
        top_right->set_fill({top_right_corner_glyph(), attr});
        centre_left->set_fill({vertical_beam_glyph(), attr});
        centre_right->set_fill({vertical_beam_glyph(), attr});
        bottom_left->set_fill({bottom_left_corner_glyph(), attr});
        bottom_centre->set_fill({horizontal_beam_glyph(), attr});
        bottom_right->set_fill({bottom_right_corner_glyph(), attr});
    }
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
solid_frame::solid_frame()
  : pimpl_(std::make_shared<impl>())
{
    auto north_beam = view(
        make_compass_layout(),
        pimpl_->top_left,   compass_layout::heading::west,
        pimpl_->top_centre, compass_layout::heading::centre,
        pimpl_->top_right,  compass_layout::heading::east);
    
    auto south_beam = view(
        make_compass_layout(),
        pimpl_->bottom_left, compass_layout::heading::west,
        pimpl_->bottom_centre, compass_layout::heading::centre,
        pimpl_->bottom_right, compass_layout::heading::east);
        
    auto west_beam = pimpl_->centre_left;
    auto east_beam = pimpl_->centre_right;
    
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
    pimpl_->set_fills();
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void solid_frame::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    pimpl_->lowlight_attribute = lowlight_attribute;
    pimpl_->set_fills();
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void solid_frame::highlight_on_focus(
    std::shared_ptr<component> const &associated_component)
{
    auto const evaluate_focus = 
        [this, wp = std::weak_ptr<component>(associated_component)]
        {
            std::shared_ptr<component> associated_component(wp.lock());

            if (associated_component)
            {
                pimpl_->is_highlighted = associated_component->has_focus();
                pimpl_->set_fills();
            }
        };

    associated_component->on_focus_set.connect(evaluate_focus);
    associated_component->on_focus_lost.connect(evaluate_focus);
    evaluate_focus();
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
