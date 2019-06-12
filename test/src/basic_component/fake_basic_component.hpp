#pragma once
#include <munin/basic_component.hpp>

// Tests run on this fake basic component that just completes the remaining
// virtual functions.
class fake_basic_component : public munin::basic_component
{
private :
    terminalpp::extent do_get_preferred_size() const override
    {
        return {0, 0};
    }
    
    void do_draw(
        munin::render_surface &surface, 
        terminalpp::rectangle const &region) const override
    {
    }
};
