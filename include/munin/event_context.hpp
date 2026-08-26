#pragma once

#include "munin/export.hpp"

namespace munin {

class MUNIN_EXPORT event_context
{
public:
    event_context() = default;
    event_context(event_context const &) = delete;
    event_context(event_context &&) = delete;
    ~event_context() = default;

    auto operator=(event_context const &) -> event_context & = delete;
    auto operator=(event_context &&) -> event_context & = delete;

    [[nodiscard]] bool register_click_interest();
    void deregister_click_interest();

    void set_click_synthesis_supported(bool supported);
    void reset_mouse_dispatch_state();
    [[nodiscard]] bool has_click_interest() const;

private:
    bool click_synthesis_supported_ = false;
    bool has_click_interest_ = false;
};

}  // namespace munin
