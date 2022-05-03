#include <munin/button.hpp>
#include <munin/console_application.hpp>
#include <munin/compass_layout.hpp>
#include <munin/filled_box.hpp>
#include <munin/image.hpp>
#include <munin/view.hpp>
#include <terminalpp/string.hpp>
#include <terminalpp/terminal.hpp>
#include <boost/asio/io_context.hpp>

using namespace terminalpp::literals;

namespace {

auto make_content(auto &button)
{
    return munin::view(
        munin::make_compass_layout(),
        munin::make_image("Hello, \\p-world!"_ets),
        munin::compass_layout::heading::centre,
        munin::view(
            munin::make_compass_layout(),
            munin::make_fill(' '),
            munin::compass_layout::heading::centre,
            button,
            munin::compass_layout::heading::east
        ),
        munin::compass_layout::heading::south);
}

auto make_behaviour()
{
    terminalpp::behaviour behaviour;
    behaviour.supports_basic_mouse_tracking = true;
    behaviour.supports_window_title_bel = true;
    return behaviour;
}

}

int main()
{
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);

    auto button = munin::make_button(" OK ");
    button->on_click.connect([&]{ 
        io_context.stop();
        work_guard.reset(); 
    });

    munin::console_application app{make_behaviour(), io_context, make_content(button)};

    app.terminal()
        << terminalpp::enable_mouse()
        << terminalpp::hide_cursor()
        << terminalpp::use_alternate_screen_buffer()
        << terminalpp::set_window_title("hello_console");
    button->set_focus();

    io_context.run();

    app.terminal()
        << terminalpp::use_normal_screen_buffer()
        << terminalpp::show_cursor()
        << terminalpp::disable_mouse();
}
