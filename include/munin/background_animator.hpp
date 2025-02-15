#pragma once
#include "munin/animator.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that executes the scheduled redraw requests on an
/// execution strand.
///
/// The expected usage of this animator is that all operations on the
/// animated component (and probably all components for a given user interface
/// also take place on the strand so that there are no race conditions between
/// animation and normal operation.  This requires co-ordination from a top-
/// level user interface component, which would be the owner of both the
/// strand and the animator.
//* =========================================================================
class MUNIN_EXPORT background_animator : public animator
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit background_animator(boost::asio::io_context::strand &strand);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~background_animator() override;

private:
    //* =====================================================================
    /// \brief Schedules a function to be called at a certain time.
    /// This is used to schedule the requested component redraws.
    //* =====================================================================
    void reset_timer(
        std::chrono::steady_clock::time_point execution_time) override;

    //* =====================================================================
    /// \brief Returns the current time
    //* =====================================================================
    [[nodiscard]] std::chrono::steady_clock::time_point do_now() const override;

    boost::asio::io_context::strand &strand_;
    boost::asio::steady_timer timer_;
};

}  // namespace munin
