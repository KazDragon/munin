#pragma once
#include <munin/component.hpp>

#include <chrono>

namespace munin {

//* =========================================================================
/// \brief An injectable interface into a render surface that allows
/// timing of animated components.
//* =========================================================================
class MUNIN_EXPORT animator  // NOLINT
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    animator();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~animator();

    //* =====================================================================
    /// \brief Schedules a redraw of the passed component
    //* =====================================================================
    void redraw_component_at(
        std::shared_ptr<component const> const &comp,
        terminalpp::rectangle bounds,
        std::chrono::steady_clock::time_point execution_time);

    //* =====================================================================
    /// \brief Returns the current time
    //* =====================================================================
    [[nodiscard]] std::chrono::steady_clock::time_point now() const;

protected:
    //* =====================================================================
    /// \brief Redraws components whose animation timers have expired.  This
    /// must be called from a derived class after its own timer has expired.
    /// The timer is set in reset_timer(execution_time).
    //* =====================================================================
    void redraw_components();

private:
    //* =====================================================================
    /// \brief Schedules a function to be called at a certain time.
    /// This is used to schedule the requested component redraws.
    //* =====================================================================
    virtual void reset_timer(
        std::chrono::steady_clock::time_point execution_time) = 0;

    //* =====================================================================
    /// \brief Returns the current time
    //* =====================================================================
    [[nodiscard]] virtual std::chrono::steady_clock::time_point do_now()
        const = 0;

    class impl;
    std::unique_ptr<impl> pimpl_;
};

}  // namespace munin
