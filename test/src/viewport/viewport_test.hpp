#include "mock/component.hpp"
#include <munin/viewport.hpp>

class a_viewport_with_mock_tracked_component
{
protected:
    std::shared_ptr<mock_component> tracked_component_ = make_mock_component();
    std::shared_ptr<munin::viewport> viewport_ = munin::make_viewport(tracked_component_);
};
