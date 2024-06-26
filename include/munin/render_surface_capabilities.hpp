#pragma once

namespace munin {

//* =========================================================================
/// \brief A class that describes the capabilities of a render surface.
//* =========================================================================
struct render_surface_capabilities
{
    virtual ~render_surface_capabilities() = default;

    //* =====================================================================
    /// \brief Returns true if the surface is known to support unicode.
    /// characters.  Attempting to render unicode on surfaces that do not
    /// support unicode may have unexpected results.
    //* =====================================================================
    [[nodiscard]] virtual bool supports_unicode() const = 0;
};

//* =========================================================================
/// \brief A class that supports the default set of render surface
/// capabilities
//* =========================================================================
class default_render_surface_capabilities : public render_surface_capabilities
{
public:
    // ======================================================================
    // SUPPORTS_UNICODE
    // ======================================================================
    [[nodiscard]] bool supports_unicode() const override
    {
        return true;
    }
};

extern default_render_surface_capabilities default_capabilities;

}  // namespace munin