#include "paint/paint_app.hpp"

#include <boost/asio/io_context.hpp>
#include <munin/console_application.hpp>
#include <terminalpp/terminal.hpp>

namespace {

auto make_behaviour() -> terminalpp::behaviour
{
    auto behaviour = terminalpp::behaviour{};
    behaviour.supports_basic_mouse_tracking = true;
    behaviour.supports_button_event_mouse_tracking = true;
    behaviour.supports_all_mouse_motion_tracking = true;
    behaviour.supports_sgr_mouse_encoding = true;
    behaviour.supports_window_title_bel = true;
    return behaviour;
}

}  // namespace

int main()
{
    auto io_context = boost::asio::io_context{};
    auto work_guard = boost::asio::make_work_guard(io_context);
    auto content = paint::make_paint_app();

    content->on_quit.connect([&] {
        io_context.stop();
        work_guard.reset();
    });

    auto app =
        munin::console_application{make_behaviour(), io_context, content};
    app.terminal() << terminalpp::enable_mouse() << terminalpp::hide_cursor()
                   << terminalpp::use_alternate_screen_buffer()
                   << terminalpp::set_window_title("paint");

    content->set_focus();
    io_context.run();

    app.terminal() << terminalpp::use_normal_screen_buffer()
                   << terminalpp::show_cursor() << terminalpp::disable_mouse();
}
