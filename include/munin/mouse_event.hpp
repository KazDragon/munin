#pragma once

#include "munin/export.hpp"

#include <terminalpp/mouse.hpp>
#include <terminalpp/point.hpp>
#include <terminalpp/virtual_key.hpp>

namespace munin {

enum class mouse_event_type
{
    button_down,
    button_up,
    click,
    out,
    in
};

struct MUNIN_EXPORT mouse_event
{
    mouse_event_type action_ = mouse_event_type::button_down;
    terminalpp::point position_;
    terminalpp::mouse::button button_ = terminalpp::mouse::button::none;
    terminalpp::vk_modifier modifiers_ = terminalpp::vk_modifier::none;

    [[nodiscard]] constexpr friend auto operator<=>(
        mouse_event const &lhs, mouse_event const &rhs) noexcept = default;
};

}  // namespace munin
