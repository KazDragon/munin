#include "munin/toggle_button.hpp"
#include "munin/filled_box.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/solid_frame.hpp"

namespace munin {

// ==========================================================================
// TOGGLE_BUTTON::IMPL
// ==========================================================================
struct toggle_button::impl
{
    std::shared_ptr<component> fill_;
    bool toggle_state_ = false;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
toggle_button::toggle_button(bool checked)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->fill_ = 
        make_fill([this](render_surface&) -> terminalpp::element
        {
            return pimpl_->toggle_state_
                 ? 'X'
                 : ' ';
        });
        
    pimpl_->toggle_state_ = checked;
    
    set_layout(make_grid_layout({1, 1}));
    add_component(make_framed_component(
        make_solid_frame(),
        pimpl_->fill_));
}

// ==========================================================================
// SET_TOGGLE_STATE
// ==========================================================================
void toggle_button::set_toggle_state(bool checked)
{
    if (checked != pimpl_->toggle_state_)
    {
        pimpl_->toggle_state_ = checked;
        on_state_changed(pimpl_->toggle_state_);
        on_redraw({
            { {pimpl_->fill_->get_position()}, {pimpl_->fill_->get_size()} }
        });
    }
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<toggle_button> make_toggle_button(bool checked)
{
    return std::make_shared<toggle_button>(checked);
}

}