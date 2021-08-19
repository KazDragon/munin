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

class terminal_reader
{
public:
    terminal_reader(munin::window &window)
      : dispatcher_(window)
    {
    }

    void operator()(terminalpp::tokens const &tokens)
    {
        for (auto const &token : tokens)
        {
            boost::apply_visitor(dispatcher_, token);
        }
    }

private:
    struct event_dispatcher : public boost::static_visitor<>
    {
        event_dispatcher(munin::window &window)
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

    event_dispatcher dispatcher_;
};

void schedule_read(
    consolepp::console &console, 
    terminalpp::terminal &terminal,
    terminal_reader &reader)
{
    console.async_read(
        [&](consolepp::bytes data)
        {
            terminal.read(reader) >> data;
            schedule_read(console, terminal, reader);
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
    terminalpp::terminal terminal{make_behaviour()};

    auto const &console_write =
        [&console](terminalpp::bytes data)
        {
            console.write(data);
        };

    auto const &repaint_window = 
        [&]()
        {
            window.repaint(canvas, terminal, console_write);
        };

    window.on_repaint_request.connect(repaint_window);
    console.on_size_changed.connect(
        [&]
        {
            auto const new_size = console.size();
            canvas.resize({new_size.width, new_size.height});
            repaint_window();
        });

    terminal.write(console_write) 
        << terminalpp::enable_mouse()
        << terminalpp::use_alternate_screen_buffer()
        << terminalpp::set_window_title("hello_console");

    auto work_guard = boost::asio::make_work_guard(io_context);
    button->on_click.connect([&]{ 
        io_context.stop();
        work_guard.reset(); 
    });
    button->set_focus();

    terminal_reader reader(window);
    schedule_read(console, terminal, reader);

    io_context.run();
    terminal.write(console_write) 
        << terminalpp::use_normal_screen_buffer()
        << terminalpp::disable_mouse();
}
