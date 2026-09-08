#pragma once

#include "munin/export.hpp"

#include <terminalpp/point.hpp>

#include <memory>

namespace munin {

class component;

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

    void capture_mouse(
        std::shared_ptr<component> const &component,
        terminalpp::point const &origin);
    [[nodiscard]] std::shared_ptr<component> captured_component() const;
    [[nodiscard]] terminalpp::point captured_origin() const;
    [[nodiscard]] bool captured_mouse_inside() const;
    void set_captured_mouse_inside(bool inside);

private:
    bool click_synthesis_supported_ = false;
    bool has_click_interest_ = false;
    std::weak_ptr<component> captured_component_;
    terminalpp::point captured_origin_;
    bool captured_mouse_inside_ = false;
};

}  // namespace munin
