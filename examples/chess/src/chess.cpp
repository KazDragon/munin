#include <munin/button.hpp>
#include <munin/compass_layout.hpp>
#include <munin/filled_box.hpp>
#include <munin/grid_layout.hpp>
#include <munin/image.hpp>
#include <munin/scroll_pane.hpp>
#include <munin/text_area.hpp>
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
    explicit terminal_reader(munin::window &window)
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
        explicit event_dispatcher(munin::window &window)
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

auto make_board()
{
    return munin::make_image({
        { "\\p-" "  " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "\\U265A " "\\p+" "  " "\\p-" "  " "\\p+" "\\U265C "_ets },
        { "\\p+" "\\U265F " "\\p-" "\\U265F " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "\\U265F " "\\p+" "\\U265F " "\\p-" "\\U265F "_ets },
        { "\\p-" "  " "\\p+" "\\U265B " "\\p-" "  " "\\p+" "  " "\\p-" "\\U265F " "\\p+" "  " "\\p-" "  " "\\p+" "  "_ets },
        { "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "\\U265F " "\\p+" "\\U2659 " "\\p-" "  " "\\p+" "  " "\\p-" "  "_ets },
        { "\\p-" "  " "\\p+" "  " "\\p-" "  " "\\p+" "\\U2655 " "\\p-" "  " "\\p+" "\\U2659 " "\\p-" "  " "\\p+" "  "_ets },
        { "\\p+" "\\U2659 " "\\p-" "\\U2659 " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "  "_ets },
        { "\\p-" "  " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "  " "\\p+" "  " "\\p-" "\\U2659 " "\\p+" "\\U2659 "_ets },
        { "\\p+" "\\U2656 " "\\p-" "\\U2658 " "\\p+" "\\U265C " "\\p-" "\\U2656 " "\\p+" "  " "\\p-" "  " "\\p+" "\\U2654 " "\\p-  "_ets }
    });
}

auto make_white_text()
{
    auto text_area = munin::make_text_area();
    text_area->insert_text(
        "e4 d4 e5 Bd3 Q\\i>x\\xd3 f4 c3 Nf3 0-0 b3 c\\i>x\\xd4 Bb2 a3 "
        "N\\i>x\\xd4 Rd1 B\\i>x\\xd4 Q\\i>x\\xd4 Kf2 Q\\i>x\\xb6 Ke2 Kd2 g3 "
        "a4 b4"_ets);
    return text_area;
}

auto make_black_text()
{
    auto text_area = munin::make_text_area();
    text_area->insert_text(
        "c6 d5 Bf5 B\\i>x\\xd3 e6 c5 Nc6 Qb6 Nh6 c\\i>x\\xd4 Nf5 Rc8 "
        "Nc\\i>x\\xd4 Bc5 N\\i>x\\xd5 B\\i>x\\xd4\\[3\\i>+\\x Rc1 R\\i>x\\xd1 "
        "a\\i>x\\xb6 Rc1 Rg1 Kd7 Rc8 Rcc1"_ets
    );
    return text_area;
}

auto make_content(auto &button)
{
    return munin::view(
        munin::make_compass_layout(),
        munin::view(
            munin::make_grid_layout({2, 1}),
            munin::view(
                munin::make_compass_layout(),
                make_board(),
                munin::compass_layout::heading::centre,
                munin::make_fill("\\[6\\U2502"_ete),
                munin::compass_layout::heading::east),
            munin::view(
                munin::make_grid_layout({1, 2}),
                munin::make_scroll_pane(make_white_text()),
                munin::make_scroll_pane(make_black_text()))),
        munin::compass_layout::heading::centre,
        munin::view(
            munin::make_compass_layout(),
            munin::make_fill("\\[6\\U2500"_ete),
            munin::compass_layout::heading::north,
            munin::make_fill(' '),
            munin::compass_layout::heading::centre,
            button,
            munin::compass_layout::heading::east),
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
        << terminalpp::set_window_title("Chess++");

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
