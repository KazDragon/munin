#include "munin/background_animator.hpp"

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/make_unique.hpp>

namespace munin {

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
background_animator::background_animator(
    boost::asio::io_context::strand &strand)
  : strand_(strand), timer_(strand.context())
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
background_animator::~background_animator() = default;

// ==========================================================================
// RESET_TIMER
// ==========================================================================
void background_animator::reset_timer(
    std::chrono::steady_clock::time_point execution_time)
{
    timer_.expires_at(execution_time);
    timer_.async_wait(
        boost::asio::bind_executor(strand_, [this](std::error_code const &ec) {
            if (!ec)
            {
                redraw_components();
            }
        }));
}

// ==========================================================================
// DO_NOW
// ==========================================================================
std::chrono::steady_clock::time_point background_animator::do_now() const
{
    return std::chrono::steady_clock::now();
}

}  // namespace munin