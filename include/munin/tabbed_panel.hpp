#pragma once

#include "munin/composite_component.hpp"
#include "odin/core.hpp"
#include <boost/optional.hpp>
#include <string>

namespace munin {

//* =========================================================================
/// \brief A class that constructs a tabbed panel.
//* =========================================================================
class MUNIN_EXPORT tabbed_panel : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    tabbed_panel();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~tabbed_panel();

    //* =====================================================================
    /// \brief Inserts a tab into the frame.
    /// \par text The text of the tab.  This may be specified using the ANSI
    /// markup notation.  It will be the identifier of the tab when called
    /// back via the on_tab_selected() signal.
    /// \par comp A component to display on this tab.
    /// \par index Optionally, the index at which the tab should be inserted.
    /// If this is not specified, then it is inserted as the right-most tab.
    //* =====================================================================
    void insert_tab(
        std::string                       const &text,
        std::shared_ptr<munin::component> const &comp,
        boost::optional<odin::u32>               index = {});

    //* =====================================================================
    /// \brief Removes a tab from the frame.
    /// \par index The index at which the tab should be removed.
    //* =====================================================================
    void remove_tab(odin::u32 index);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created tabbed panel
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<tabbed_panel> make_tabbed_panel();

}
