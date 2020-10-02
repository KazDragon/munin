#pragma once
#include "munin/animator.hpp"
#include <boost/asio/io_context.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that executes the scheduled redraw requests using a 
/// (shared) background thread.
//* =========================================================================
class MUNIN_EXPORT background_animator : public animator
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    background_animator(boost::asio::io_context &io_context);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~background_animator() override;

private:
    //* =====================================================================
    /// \brief Schedules a function to be called at a certain time.
    /// This is used to schedule the requested component redraws.
    //* =====================================================================
    void do_call_function_at(
        std::function<void ()> const &fn,
        std::chrono::steady_clock::time_point execution_time) override;

    //* =====================================================================
    /// \brief Returns the current time
    //* =====================================================================
    std::chrono::steady_clock::time_point do_now() const override;

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
