#include <consolepp/console.hpp>
#include <munin/console_application.hpp>
#include <munin/window.hpp>
#include <terminalpp/terminal.hpp>

#include <utility>

namespace munin {
namespace {

void schedule_read(terminalpp::terminal &terminal, window &wnd)
{
    terminal.async_read([&](terminalpp::tokens tokens) {
        for (auto const &token : tokens)
        {
            std::visit([&wnd](auto const &event) { wnd.event(event); }, token);
        }

        schedule_read(terminal, wnd);
    });
}

}  // namespace

// ==========================================================================
// CONSOLE_APPLICATION::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct console_application::impl
{
    impl(
      terminalpp::behaviour const &behaviour,
      boost::asio::io_context &io_context,
      std::shared_ptr<component> content)
    : console_{io_context},
      terminal_{console_, behaviour},
      window_{terminal_, std::move(content)},
      canvas_{{console_.size().width, console_.size().height}}
    {
        window_.on_repaint_request.connect(
            [this] { window_.repaint(canvas_); });

        console_.on_size_changed.connect([this] {
            canvas_.resize({console_.size().width, console_.size().height});
            window_.repaint(canvas_);
        });

        schedule_read(terminal_, window_);
    }

    consolepp::console console_;     // NOLINT
    terminalpp::terminal terminal_;  // NOLINT
    window window_;                  // NOLINT
    terminalpp::canvas canvas_;      // NOLINT
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

}  // namespace munin
