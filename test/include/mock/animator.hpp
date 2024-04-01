#pragma once
#include <gmock/gmock.h>
#include <munin/animator.hpp>

class mock_animator : public munin::animator
{
public:
    //* =====================================================================
    /// \brief Schedules a function to be called in an amount of time.
    /// This is used to schedule the requested component redraws.
    //* =====================================================================
    MOCK_METHOD1(reset_timer, void(std::chrono::steady_clock::time_point));

    //* =====================================================================
    /// \brief Returns the current time
    //* =====================================================================
    MOCK_CONST_METHOD0(do_now, std::chrono::steady_clock::time_point());

    //* =====================================================================
    /// \brief Requests a redraw of all expired components
    //* =====================================================================
    using munin::animator::redraw_components;
};

//* =========================================================================
/// \brief Makes a mock animator
//* =========================================================================
std::shared_ptr<mock_animator> make_mock_animator();
