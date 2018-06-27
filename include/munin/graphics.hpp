#pragma once

#include "munin/export.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that describes the desirable graphical attributes of
/// the rendering device. 
//* =========================================================================
class MUNIN_EXPORT graphics
{
public :
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~graphics() = default;

    //* =====================================================================
    /// \brief Returns true iff unicode should be used for drawing
    /// characters
    //* =====================================================================
    virtual bool supports_unicode() const = 0;
};

//* =========================================================================
/// \brief Retrieves the current graphics object.
//* =========================================================================
MUNIN_EXPORT
graphics& get_graphics();

//* =========================================================================
/// \brief Sets the current graphics object.
///
/// Setting an null value will install the default graphics object.
/// Note: a graphics object is associated with a particular thread.  This
/// allows independent components with different graphics objects to be 
/// processed concurrently.
///
/// It is the caller's responsibility to ensure that gr is lives until either
/// this thread is terminated, or set_graphics is called again on this 
/// thread.
//* =========================================================================
MUNIN_EXPORT
void set_graphics(graphics* gr);

}
