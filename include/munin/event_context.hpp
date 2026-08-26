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
};

}  // namespace munin
