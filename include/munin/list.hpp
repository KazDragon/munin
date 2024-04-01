#include "munin/basic_component.hpp"

#include <terminalpp/string.hpp>

#include <optional>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief A class that displays a list of elements.
//* =========================================================================
class MUNIN_EXPORT list : public basic_component  // NOLINT
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    list();

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    ~list() override;

    //* =====================================================================
    /// \brief Gets the index of the selected item, if such an item exists.
    //* =====================================================================
    [[nodiscard]] std::optional<int> get_selected_item_index() const;

    //* =====================================================================
    /// \brief Selects an item in the list.  If the passed index is not
    /// initialized, then de-selects any selected item.
    //* =====================================================================
    void select_item(std::optional<int> const &index);

    //* =====================================================================
    /// \brief Sets the items in the list.
    //* =====================================================================
    void set_items(std::vector<terminalpp::string> const &items);

    //* =====================================================================
    /// \fn on_item_changed
    /// \brief Connect to this signal to receive notifications when the
    /// selected item has changed.
    //* =====================================================================
    boost::signals2::signal<void()> on_item_changed;

private:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::point do_get_cursor_position() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(render_surface &surface, terminalpp::rectangle const &region)
        const override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created list
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<list> make_list();

}  // namespace munin