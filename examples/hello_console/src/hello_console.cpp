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

namespace {

struct window_event_dispatcher : public boost::static_visitor<>
{
    window_event_dispatcher(munin::window &window)
      : window_{window}
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

class hello_console
{
public:
    void run()
    {
        terminalpp::canvas canvas{{
            console_.size().width, console_.size().height}};

        auto const &repaint_window = [&]() { window_.repaint(canvas); };
        window_.on_repaint_request.connect(repaint_window);

        console_.on_size_changed.connect(
            [&]
            {
                auto const new_size = console_.size();
                canvas.resize({new_size.width, new_size.height});
                repaint_window();
            });

        terminal_
            << terminalpp::enable_mouse()
            << terminalpp::hide_cursor()
            << terminalpp::use_alternate_screen_buffer()
            << terminalpp::set_window_title("hello_console");

        auto work_guard = boost::asio::make_work_guard(io_context_);
        button_->on_click.connect([&]{ 
            io_context_.stop();
            work_guard.reset(); 
        });
        button_->set_focus();

        schedule_read(console_, terminal_);

        io_context_.run();
        terminal_
            << terminalpp::use_normal_screen_buffer()
            << terminalpp::show_cursor()
            << terminalpp::disable_mouse();
    }

private:
    boost::asio::io_context io_context_;
    consolepp::console console_{io_context_};

    terminalpp::terminal terminal_{
        [this](terminalpp::tokens tokens) {
            for (const auto &token : tokens)
            {
                boost::apply_visitor(event_dispatcher_, token);
            }
        },
        [this](terminalpp::bytes data) {
            console_.write(data);
        },
        make_behaviour()
    };

    std::shared_ptr<munin::button> button_{munin::make_button(" OK ")};
    munin::window window_{terminal_, make_content(button_)};
    window_event_dispatcher event_dispatcher_{window_};
};

}

int main()
{
    hello_console app;
    app.run();
}
