#include "munin/basic_component.hpp"
#include <terminalpp/string.hpp>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief A class that displays a list of elements.
//* =========================================================================
class MUNIN_EXPORT list : public basic_component
{
public:
    //* =====================================================================
    /// \brief Gets the index of the selected item, if such an item exists.
    //* =====================================================================
    boost::optional<int> get_selected_item_index() const;

    //* =====================================================================
    /// \brief Sets the selected item.  If the parameter is not initialized,
    /// or the index is not valid, then de-selects any selected item.
    //* =====================================================================
    void set_selected_item_index(boost::optional<int> const &index);

    //* =====================================================================
    /// \brief Sets the items in the list.
    //* =====================================================================
    void set_items(std::vector<terminalpp::string> const &items);

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