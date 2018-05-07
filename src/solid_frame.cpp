#include <munin/solid_frame.hpp>
#include <munin/compass_layout.hpp>
#include <munin/filled_box.hpp>
#include <munin/view.hpp>

namespace munin {

struct solid_frame::impl
{
    std::shared_ptr<munin::filled_box> top_left      = munin::make_fill('+');
    std::shared_ptr<munin::filled_box> top_centre    = munin::make_fill('-');
    std::shared_ptr<munin::filled_box> top_right     = munin::make_fill('+');
    std::shared_ptr<munin::filled_box> centre_left   = munin::make_fill('|');
    std::shared_ptr<munin::filled_box> centre_right  = munin::make_fill('|');
    std::shared_ptr<munin::filled_box> bottom_left   = munin::make_fill('+');
    std::shared_ptr<munin::filled_box> bottom_centre = munin::make_fill('-');
    std::shared_ptr<munin::filled_box> bottom_right  = munin::make_fill('+');
};

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

void solid_frame::set_highlight(bool highlight)
{
    static auto const lowlight_attribute  = terminalpp::attribute();
    static auto const highlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    auto const &attr = 
        highlight ? highlight_attribute : lowlight_attribute;
        
    pimpl_->top_left->set_fill({'+', attr});
    pimpl_->top_centre->set_fill({'-', attr});
    pimpl_->top_right->set_fill({'+', attr});
    pimpl_->centre_left->set_fill({'|', attr});
    pimpl_->centre_right->set_fill({'|', attr});
    pimpl_->bottom_left->set_fill({'+', attr});
    pimpl_->bottom_centre->set_fill({'-', attr});
    pimpl_->bottom_right->set_fill({'+', attr});
}

std::shared_ptr<solid_frame> make_solid_frame()
{
    return std::make_shared<solid_frame>();
}
    
}