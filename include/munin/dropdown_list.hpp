#pragma once

#include "munin/export.hpp"
#include "munin/composite_component.hpp"
#include "odin/core.hpp"

namespace terminalpp {
    class string;
}

namespace munin {

//* =========================================================================
/// \brief A class that models a dropdown_list of items.
//* =========================================================================
class MUNIN_EXPORT dropdown_list : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    dropdown_list();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~dropdown_list();

    //* =====================================================================
    /// \brief Sets the items in the drop-down list.
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
    //* =====================================================================
    boost::signal<
        void (odin::s32)
    > on_item_changed;

protected :
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
/// \brief Returns a newly created dropdown list
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<dropdown_list> make_dropdown_list();

}
