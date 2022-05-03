#include <munin/console_application.hpp>
#include <munin/window.hpp>
#include <terminalpp/terminal.hpp>
#include <consolepp/console.hpp>
#include <boost/make_unique.hpp>
#include <utility>

namespace munin { namespace {

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

}

// ==========================================================================
// CONSOLE_APPLICATION::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct console_application::impl
{
    impl(terminalpp::behaviour const &behaviour,
        boost::asio::io_context &io_context,
        std::shared_ptr<component> content)
      : terminal_{
            [this](terminalpp::tokens tokens) {
                for (auto const &token : tokens)
                {
                    boost::apply_visitor(dispatcher_, token);
                }
            },
            [this](terminalpp::bytes data) {
                console_.write(data);
            },
            behaviour
        },
        console_{io_context},
        window_{terminal_, std::move(content)},
        dispatcher_{window_},
        canvas_{{console_.size().width, console_.size().height}}
    {
        window_.on_repaint_request.connect([this]{ 
            window_.repaint(canvas_); 
        });

        console_.on_size_changed.connect([this] {
            canvas_.resize({console_.size().width, console_.size().height});
            window_.repaint(canvas_);
        });

        schedule_read(console_, terminal_);
    }

    terminalpp::terminal terminal_;
    consolepp::console console_;
    window window_;
    window_event_dispatcher dispatcher_;
    terminalpp::canvas canvas_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
console_application::console_application(
    terminalpp::behaviour const &beh,
    boost::asio::io_context &io_context,
    std::shared_ptr<component> content)
  : pimpl_{std::make_unique<impl>(beh, io_context, std::move(content))}
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
console_application::~console_application() = default;

// ==========================================================================
// CONSOLE_APPLICATION::TERMINAL
// ==========================================================================
terminalpp::terminal &console_application::terminal()
{
    return pimpl_->terminal_;
}

}
