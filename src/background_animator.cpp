#include "munin/background_animator.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/make_unique.hpp>
#include <mutex>
#include <queue>
#include <thread>

namespace munin {

namespace {

struct delayed_function
{
    std::function<void ()>  fn;
    std::chrono::steady_clock::time_point time;
};

struct compare_delayed_function
{
    bool operator()(delayed_function const &lhs, delayed_function const &rhs)
    {
        // In the priority queue, we want the next item (highest priority)
        // to be the one with the lowest time.
        return lhs.time > rhs.time;
    }
};

}

// ==========================================================================
// BACKGROUND_ANIMATOR::IMPLEMENTATION_STRUCTURE
// ==========================================================================
struct background_animator::impl
  : public std::enable_shared_from_this<impl>
{
public:
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(boost::asio::io_context &io_context)
      : io_context_(io_context),
        steady_timer_(io_context_)
    {
    }

    // ======================================================================
    // DESTRUCTOR
    // ======================================================================
    ~impl()
    {
        steady_timer_.cancel();
    }

    // ======================================================================
    // CALL_FUNCTION_AT
    // ======================================================================
    void call_function_at(
        std::function<void ()> const &fn,
        std::chrono::steady_clock::time_point const execution_time)
    {
        std::lock_guard<std::recursive_mutex> guard{queue_mutex_};
        queue_.push({fn, execution_time});
        schedule_next_execution();
    }

    // ======================================================================
    // NOW
    // ======================================================================
    std::chrono::steady_clock::time_point now() const
    {
        return std::chrono::steady_clock::now();
    }

private:
    // ======================================================================
    // SCHEDULE_NEXT_EXECUTION
    // ======================================================================
    void schedule_next_execution()
    {
        // Assumes a lock is held for queue_mutex_.

        // Set the timer to expire at the time the next delayed function
        // is set, and then set up a callback to execute any waiting
        // functions at that time.  The call to async_wait may be executed 
        // immediately if the wait time has already passed, and this is why 
        // the mutex must be recursive.
        steady_timer_.expires_at(queue_.top().time);
        steady_timer_.async_wait(
            [wp = std::weak_ptr<impl>(shared_from_this())](
                boost::system::error_code const &error)
            {
                // It is plausible that this callback is executed during the
                // destruction of the impl object without errors, in the case
                // that the destructor is being run but hasn't cancelled the
                // timer yet.  In this case, we must not execute anything on
                // that object.
                auto const &pthis = wp.lock();

                if (!error && pthis != nullptr)
                {
                    std::lock_guard<std::recursive_mutex> guard(
                        pthis->queue_mutex_);
                    pthis->execute_waiting_functions();
                }
            });
    }

    // ======================================================================
    // EXECUTE_WAITING_FUNCTIONS
    // ======================================================================
    void execute_waiting_functions()
    {
        // Assumes a lock is held for queue_mutex_.

        // Execute any functions whose expiry time has elapsed.
        while (!queue_.empty() && queue_.top().time <= now())
        {
            queue_.top().fn();
            queue_.pop();
        }

        // If there are functions that remain to be executed, set up the 
        // timer again.
        if (!queue_.empty())
        {
            schedule_next_execution();
        }
    }

    boost::asio::io_context &io_context_;

    std::recursive_mutex queue_mutex_;

    using delayed_function_priority_queue = std::priority_queue<
        delayed_function, 
        std::vector<delayed_function>,
        compare_delayed_function
    >;
    
    delayed_function_priority_queue queue_;

    boost::asio::steady_timer steady_timer_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
background_animator::background_animator(boost::asio::io_context &io_context)
  : pimpl_(std::make_shared<impl>(io_context))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
background_animator::~background_animator() = default;

// ==========================================================================
// DO_CALL_FUNCTION_AT
// ==========================================================================
void background_animator::do_call_function_at(
    std::function<void ()> const &fn,
    std::chrono::steady_clock::time_point const execution_time)
{
    pimpl_->call_function_at(fn, execution_time);
}

// ==========================================================================
// DO_NOW
// ==========================================================================
std::chrono::steady_clock::time_point background_animator::do_now() const
{
    return pimpl_->now();
}

}