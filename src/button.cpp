#include "munin/button.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/image.hpp"
#include "munin/solid_frame.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

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
    else
    {
        auto const* vk =
            boost::any_cast<terminalpp::virtual_key>(&ev);
            
        if (vk != nullptr)
        {
            if (vk->key == terminalpp::vk::enter
             || vk->key == terminalpp::vk::space)
            {
                on_click();
            }
        }
    }
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<button> make_button(terminalpp::string const &text)
{
    return std::make_shared<button>(text);
}

}
