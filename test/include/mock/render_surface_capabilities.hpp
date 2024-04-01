#pragma once

#include <gmock/gmock.h>
#include <munin/render_surface_capabilities.hpp>

//* =========================================================================
/// \brief A class that describes the capabilities of a render surface.
//* =========================================================================
struct mock_render_surface_capabilities : munin::render_surface_capabilities
{
    //* =====================================================================
    /// \brief Returns true if the surface is known to support unicode.
    /// characters.  Attempting to render unicode on surfaces that do not
    /// support unicode may have unexpected results.
    //* =====================================================================
    MOCK_CONST_METHOD0(supports_unicode, bool());
};
