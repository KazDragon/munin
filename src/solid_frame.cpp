#include <munin/solid_frame.hpp>
#include <munin/compass_layout.hpp>
#include <munin/filled_box.hpp>
#include <munin/view.hpp>

namespace munin {

struct solid_frame::impl
{
    terminalpp::attribute lowlight_attribute = {};
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold};

    static constexpr terminalpp::glyph corner_glyph          = '+';
    static constexpr terminalpp::glyph horizontal_beam_glyph = '-';
    static constexpr terminalpp::glyph vertical_beam_glyph   = '|';
    
    std::shared_ptr<munin::filled_box> top_left = 
        munin::make_fill({corner_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> top_centre = 
        munin::make_fill({horizontal_beam_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> top_right = 
        munin::make_fill({corner_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> centre_left = 
        munin::make_fill({vertical_beam_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> centre_right = 
        munin::make_fill({vertical_beam_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_left = 
        munin::make_fill({corner_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_centre = 
        munin::make_fill({horizontal_beam_glyph, lowlight_attribute});
    std::shared_ptr<munin::filled_box> bottom_right = 
        munin::make_fill({corner_glyph, lowlight_attribute});
    
    bool is_highlighted = false;
    
    void set_fills()
    {
        auto const &attr = is_highlighted ? highlight_attribute : lowlight_attribute;
        
        top_left->set_fill({corner_glyph, attr});
        top_centre->set_fill({horizontal_beam_glyph, attr});
        top_right->set_fill({corner_glyph, attr});
        centre_left->set_fill({vertical_beam_glyph, attr});
        centre_right->set_fill({vertical_beam_glyph, attr});
        bottom_left->set_fill({corner_glyph, attr});
        bottom_centre->set_fill({horizontal_beam_glyph, attr});
        bottom_right->set_fill({corner_glyph, attr});
    }
};

constexpr terminalpp::glyph solid_frame::impl::corner_glyph;
constexpr terminalpp::glyph solid_frame::impl::horizontal_beam_glyph;
constexpr terminalpp::glyph solid_frame::impl::vertical_beam_glyph;

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

solid_frame::~solid_frame()
{
}

void solid_frame::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    pimpl_->highlight_attribute = highlight_attribute;
    pimpl_->set_fills();
}

void solid_frame::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    pimpl_->lowlight_attribute = lowlight_attribute;
    pimpl_->set_fills();
}

void solid_frame::set_highlight(bool highlight)
{
    pimpl_->is_highlighted = highlight;
    pimpl_->set_fills();
}

std::shared_ptr<solid_frame> make_solid_frame()
{
    return std::make_shared<solid_frame>();
}
    
}
