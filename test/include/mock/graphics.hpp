#pragma once

#include <munin/graphics.hpp>
#include <gmock/gmock.h>

class mock_graphics : public munin::graphics
{
public :
    //* =====================================================================
    /// \brief Returns true iff unicode should be used for drawing
    /// characters
    //* =====================================================================
    MOCK_CONST_METHOD0(supports_unicode, bool ());
};
