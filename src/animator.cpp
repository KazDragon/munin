#include "munin/animator.hpp"

#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/iterator_range.hpp>

#include <deque>
#include <memory>

namespace munin {

// ==========================================================================
// ANIMATOR::IMPLEMENTATION STRUCTURE
// ==========================================================================
class animator::impl
{
public:
    struct animation_request
    {
        std::weak_ptr<component const> comp;
        terminalpp::rectangle bounds;
        std::chrono::steady_clock::time_point execution_time;
    };

    // ======================================================================
    // LOWER_BOUND
    // ======================================================================
    auto lower_bound(std::chrono::steady_clock::time_point time)
    {
        return boost::range::lower_bound(
            animation_requests, time, compare_execution_time);
    }

    // ======================================================================
    // COMPARE_EXECUTION_TIME
    // ======================================================================
    static bool compare_execution_time(
        animation_request const &request,
        std::chrono::steady_clock::time_point execution_time)
    {
        return request.execution_time <= execution_time;
    }

    std::deque<animation_request> animation_requests;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
animator::animator() : pimpl_(std::make_unique<impl>())
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
animator::~animator() = default;

// ==========================================================================
// REDRAW_COMPONENT_AT
// ==========================================================================
void animator::redraw_component_at(
    std::shared_ptr<component const> const &comp,
    terminalpp::rectangle bounds,
    std::chrono::steady_clock::time_point const execution_time)
{
    auto const component_matches =
        [comp](impl::animation_request const &request) {
            return request.comp.lock() == comp;
        };

    // Remove any scheduled redraws for this particular component. There
    // should be at most one request for a given component at any given time.
    boost::range::remove_erase_if(
        pimpl_->animation_requests, component_matches);

    // Insert the request into the collection, maintaining an ordering by
    // execution time.
    pimpl_->animation_requests.insert(
        pimpl_->lower_bound(execution_time),
        impl::animation_request{comp, bounds, execution_time});

    // The 0th request is the one that must happen next, so reset our timer
    // for that moment.
    reset_timer(pimpl_->animation_requests[0].execution_time);
}

// ==========================================================================
// REDRAW_COMPONENTS
// ==========================================================================
void animator::redraw_components()
{
    auto const current_time = now();

    // Select the requests whose timers have expired and send their redraw
    // events, and then remove the requests.  Note that their event is
    // asynchronous to the actual component redraws, so the collection of
    // requests is unchanged by this action.
    auto const expired_requests = boost::make_iterator_range(
        pimpl_->animation_requests.begin(), pimpl_->lower_bound(current_time));

    for (auto const &request : expired_requests)
    {
        if (auto const &comp = request.comp.lock(); comp)
        {
            comp->on_redraw({request.bounds});
        }
    }

    boost::erase(pimpl_->animation_requests, expired_requests);

    // Finally, if there are remaining animation requests, then set the timer
    // so that it would expire on the next such request.
    if (!pimpl_->animation_requests.empty())
    {
        reset_timer(pimpl_->animation_requests[0].execution_time);
    }
}

// ==========================================================================
// NOW
// ==========================================================================
std::chrono::steady_clock::time_point animator::now() const
{
    return do_now();
}

}  // namespace munin