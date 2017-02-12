#pragma once

#include "munin/export.hpp"
#include "munin/composite_component.hpp"
#include "odin/core.hpp"

namespace munin {

//* =========================================================================
/// \brief A base class that takes a series of components and uses them to
/// construct a frame.
/// \par ATTRIBUTE_PEN
/// Set the ATTRIBUTE_PEN attribute to an value of type munin::attribute.
/// This will be passed down to subcomponents.  You can use this, for
/// example, to show a border highlight when the interior component has
/// focus.
//* =========================================================================
class MUNIN_EXPORT frame : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Returns the height of the top border
    //* =====================================================================
    odin::s32 get_top_border_height() const;

    //* =====================================================================
    /// \brief Returns the height of the bottom border
    //* =====================================================================
    odin::s32 get_bottom_border_height() const;

    //* =====================================================================
    /// \brief Returns the width of the left border
    //* =====================================================================
    odin::s32 get_left_border_width() const;

    //* =====================================================================
    /// \brief Returns the width of the right border
    //* =====================================================================
    odin::s32 get_right_border_width() const;

protected :
    //* =====================================================================
    /// \brief Called by get_top_border_height.  Derived classes must
    /// override this function in order to retrieve the top border height in
    /// a custom manner.
    //* =====================================================================
    virtual odin::s32 do_get_top_border_height() const = 0;

    //* =====================================================================
    /// \brief Called by get_bottom_border_height.  Derived classes must
    /// override this function in order to retrieve the bottomborder height
    /// int a custom manner.
    //* =====================================================================
    virtual odin::s32 do_get_bottom_border_height() const = 0;

    //* =====================================================================
    /// \brief Called by get_left_border_width.  Derived classes must
    /// override this function in order to retrieve the left border width in
    /// a custom manner.
    //* =====================================================================
    virtual odin::s32 do_get_left_border_width() const = 0;

    //* =====================================================================
    /// \brief Called by get_right_border_width.  Derived classes must
    /// override this function in order to retrieve the right border width in
    /// a custom manner.
    //* =====================================================================
    virtual odin::s32 do_get_right_border_width() const = 0;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
