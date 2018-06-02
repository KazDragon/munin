#include "munin/framed_component.hpp"

namespace munin {

framed_component::framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
{
}

framed_component::~framed_component()
{
}

std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<framed_component>(frame, inner_component);
}

}
