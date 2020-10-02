#include "munin/animator.hpp"

namespace munin {

null_animator default_animator;

// ==========================================================================
// REDRAW_COMPONENT_IN
// ==========================================================================
void animator::redraw_component_at(
        std::shared_ptr<component const> const &comp,
        terminalpp::rectangle bounds,
        std::chrono::steady_clock::time_point const execution_time)
{
    do_call_function_at(
        [wcomp = std::weak_ptr<component const>(comp), bounds]()
        {
            auto comp = wcomp.lock();

            if (comp)
            {
                comp->on_redraw({{bounds.origin, bounds.size}});
            }
        },
        execution_time);
}

// ==========================================================================
// NOW
// ==========================================================================
std::chrono::steady_clock::time_point animator::now() const
{
    return do_now();
}

}