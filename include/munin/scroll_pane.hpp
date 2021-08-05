#pragma once
#include "munin/framed_component.hpp"
#include <memory>

namespace munin {

class MUNIN_EXPORT scroll_pane : public framed_component
{
public:
    scroll_pane(std::shared_ptr<component> const &inner_component);
    ~scroll_pane();

private:
    scroll_pane(
        std::shared_ptr<component> const &frame,
        std::shared_ptr<component> const &viewport,
        std::shared_ptr<component> const &inner_component);

    struct impl;
    std::unique_ptr<impl> pimpl_;
};


MUNIN_EXPORT
std::shared_ptr<scroll_pane> make_scroll_pane(std::shared_ptr<component> const &inner_component);

}
