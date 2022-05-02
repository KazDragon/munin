#include <munin/button.hpp>
#include <munin/compass_layout.hpp>
#include <munin/filled_box.hpp>
#include <munin/image.hpp>
#include <munin/view.hpp>
#include <munin/window.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/string.hpp>
#include <terminalpp/terminal.hpp>
#include <consolepp/console.hpp>
#include <boost/asio/io_context.hpp>

using namespace terminalpp::literals;

struct window_event_dispatcher : public boost::static_visitor<>
{
    explicit window_event_dispatcher(munin::window &window)
      : window_(window)
    {
    }

    template <class Event>
    void operator()(Event const &event)
    {
        window_.event(event);
    }

    munin::window &window_;
};

void schedule_read(consolepp::console &console, terminalpp::terminal &terminal)
{
    console.async_read(
        [&](consolepp::bytes data)
        {
            terminal >> data;
            schedule_read(console, terminal);
        });
}

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

int main()
{
    auto button = munin::make_button(" OK ");
    munin::window window{make_content(button)};
    
    boost::asio::io_context io_context;
    consolepp::console console{io_context};
    terminalpp::canvas canvas{{console.size().width, console.size().height}};
    window_event_dispatcher event_dispatcher{window};

    terminalpp::terminal terminal{
        [&](terminalpp::tokens tokens) {
            for (const auto& token : tokens)
            {
                boost::apply_visitor(event_dispatcher, token);
            }
        },
        [&](terminalpp::bytes data) {
            console.write(data);
        },
        make_behaviour()
    };

    auto const &repaint_window = [&]() { window.repaint(canvas, terminal); };

    window.on_repaint_request.connect(repaint_window);
    console.on_size_changed.connect(
        [&]
        {
            auto const new_size = console.size();
            canvas.resize({new_size.width, new_size.height});
            repaint_window();
        });

    terminal
        << terminalpp::enable_mouse()
        << terminalpp::hide_cursor()
        << terminalpp::use_alternate_screen_buffer()
        << terminalpp::set_window_title("hello_console");

    auto work_guard = boost::asio::make_work_guard(io_context);
    button->on_click.connect([&]{ 
        io_context.stop();
        work_guard.reset(); 
    });
    button->set_focus();

    schedule_read(console, terminal);

    io_context.run();
    terminal
        << terminalpp::use_normal_screen_buffer()
        << terminalpp::show_cursor()
        << terminalpp::disable_mouse();
}
