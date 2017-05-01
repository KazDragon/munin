#include "munin/button.hpp"
#include "munin/aligned_layout.hpp"
#include "munin/background_fill.hpp"
#include "munin/container.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/image.hpp"
#include "munin/solid_frame.hpp"
#include "munin/view.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

namespace munin {

// ==========================================================================
// BUTTON::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct button::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl()
    {
    }
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
button::button(terminalpp::string const &caption)
    : pimpl_(std::make_shared<impl>())
{
    auto image = munin::make_image(caption);
    image->set_can_focus(true);

    auto content = get_container();
    content->set_layout(munin::make_grid_layout(1, 1));
    content->add_component(
        munin::make_framed_component(
            munin::make_solid_frame(),
            munin::view(
                munin::make_aligned_layout(),
                image, munin::alignment_hcvc)));
    content->set_layout(munin::make_grid_layout(1, 1), munin::LOWEST_LAYER);
    content->add_component(
        munin::make_background_fill(), {}, munin::LOWEST_LAYER);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
button::~button()
{
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void button::do_event(boost::any const &event)
{
    auto vk = boost::any_cast<terminalpp::virtual_key>(&event);

    if (vk)
    {
        if (vk->key == terminalpp::vk::space
         || vk->key == terminalpp::vk::enter)
        {
            on_click();
        }
    }

    auto report = boost::any_cast<terminalpp::ansi::mouse::report>(&event);

    if (report
     && report->button_ == terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN)
    {
        on_click();
    }
}

// ==========================================================================
// MAKE_BUTTON
// ==========================================================================
std::shared_ptr<button> make_button(terminalpp::string const &caption)
{
    return std::make_shared<button>(caption);
}


}

