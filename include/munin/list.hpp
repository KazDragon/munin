#include "munin/basic_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that displays a list of elements.
//* =========================================================================
class MUNIN_EXPORT list : public basic_component
{
private:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(
        render_surface &surface,
        terminalpp::rectangle const &region) const override;
};

//* =========================================================================
/// \brief Returns a newly created list
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<list> make_list();

}