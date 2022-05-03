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

class chess
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
    chess app;
    app.run();
}
