#include <munin/titled_frame.hpp>
#include "mock/render_surface_capabilities.hpp"

class a_titled_frame_with_no_unicode_support : public testing::Test
{
protected :
    a_titled_frame_with_no_unicode_support()
    {
        using testing::Return;
        
        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(false));
    }

    testing::NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::titled_frame frame_{"title"};
};

using a_titled_frame = a_titled_frame_with_no_unicode_support;

class a_titled_frame_with_unicode_support : public testing::Test
{
protected :
    a_titled_frame_with_unicode_support()
    {
        using testing::Return;
        
        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(true));
    }

    testing::NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::titled_frame frame_{"title"};
};

