#pragma once

#include "munin/export.hpp"
#include "munin/basic_component.hpp"
#include "odin/core.hpp"

namespace terminalpp {
    class string;
}

namespace munin {

//* =========================================================================
/// \brief A class that models a list of items.
//* =========================================================================
class MUNIN_EXPORT list : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    list();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~list();

    //* =====================================================================
    /// \brief Sets the items in the list.
    //* =====================================================================
    void set_items(std::vector<terminalpp::string> const &items);

    //* =====================================================================
    /// \brief Selects an item with the given index.
    /// \param selected_item the item to select, or -1 for no item.
    //* =====================================================================
    void set_item_index(odin::s32 index);

    //* =====================================================================
    /// \brief Gets the index of the currently selected item, or -1 if no
    /// item is selected.
    //* =====================================================================
    odin::s32 get_item_index() const;

    //* =====================================================================
    /// \brief Gets the value of the currently selected item.
    //* =====================================================================
    terminalpp::string get_item() const;

    //* =====================================================================
    /// \fn on_item_changed
    /// \brief Connect to this signal to receive updates about when the
    /// selected item changes.
    /// \par index The index that the item was changed from (you can query
    /// the component if you need to know the one it changed to.)
    //* =====================================================================
    boost::signal<
        void (odin::s32 index)
    > on_item_changed;

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual terminalpp::point do_get_cursor_position() const;

    //* =====================================================================
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual void do_set_cursor_position(terminalpp::point const &position);

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param ctx the context in which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    virtual void do_draw(
        context         &ctx
      , rectangle const &region);

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created list
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<list> make_list();

}
