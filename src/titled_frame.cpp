#include "munin/titled_frame.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/compass_layout.hpp"
#include "munin/image.hpp"
#include "munin/view.hpp"

namespace munin {

struct titled_frame::impl
{
    terminalpp::attribute lowlight_attribute;
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold};
    terminalpp::attribute *current_attribute = &lowlight_attribute;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
titled_frame::titled_frame(terminalpp::string const &title)
  : pimpl_(std::make_shared<impl>())
{
    auto &attr = pimpl_->current_attribute;
    
    auto title_piece = view(
        make_compass_layout(),
        make_fill(' '), compass_layout::heading::west,
        make_image(title), compass_layout::heading::centre,
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
                pimpl_->current_attribute = associated_component->has_focus()
                                          ? &pimpl_->highlight_attribute
                                          : &pimpl_->lowlight_attribute;
            }
        };

    associated_component->on_focus_set.connect(evaluate_focus);
    associated_component->on_focus_lost.connect(evaluate_focus);
    evaluate_focus();
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
