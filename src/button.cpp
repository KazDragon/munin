#include "munin/button.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/image.hpp"
#include "munin/solid_frame.hpp"
#include <terminalpp/ansi/mouse.hpp>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
button::button(terminalpp::string text)
{
    set_layout(make_grid_layout({1, 1}));
    add_component(make_framed_component(
        make_solid_frame(),
        make_image(text)));
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void button::do_event(boost::any const &ev)
{
    auto const* report = 
        boost::any_cast<terminalpp::ansi::mouse::report>(&ev);
        
    if (report != nullptr)
    {
        if (report->button_ == terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN)
        {
            on_click();
        }
    }
}
    
}
