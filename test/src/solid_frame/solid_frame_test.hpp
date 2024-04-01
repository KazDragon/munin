#pragma once
#include "mock/render_surface_capabilities.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <munin/solid_frame.hpp>

class a_solid_frame_with_no_unicode_support : public testing::Test
{
protected:
    a_solid_frame_with_no_unicode_support()
    {
        using testing::Return;

        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(false));
    }

    testing::NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::solid_frame frame_;
};

using a_solid_frame = a_solid_frame_with_no_unicode_support;

class a_solid_frame_with_unicode_support : public testing::Test
{
protected:
    a_solid_frame_with_unicode_support()
    {
        using testing::Return;

        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(true));
    }

    testing::NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::solid_frame frame_;
};
