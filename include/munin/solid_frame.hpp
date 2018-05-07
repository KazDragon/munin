#pragma once

#include <munin/composite_component.hpp>

namespace munin {
    
class MUNIN_EXPORT solid_frame : public composite_component
{
public :
    solid_frame();
    ~solid_frame();
    
    void set_highlight(bool highlight);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

MUNIN_EXPORT
std::shared_ptr<solid_frame> make_solid_frame();

}