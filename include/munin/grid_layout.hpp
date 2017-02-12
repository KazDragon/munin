#pragma once

#include "munin/layout.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a grid-like manner.  Each cell of the grid has an identical size.
/// Components added to the grid will be displayed left-to-right, top-to-
/// bottom.
//* =========================================================================
class MUNIN_EXPORT grid_layout : public layout
{
public :
    //* =====================================================================
    /// \brief Constructor
    /// \param rows The number of rows in this grid.
    /// \param columns The number of columns in this grid.
    //* =====================================================================
    grid_layout(odin::u32 rows, odin::u32 columns);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~grid_layout();

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any >                const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints
      , terminalpp::extent                             size) override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created grid layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_grid_layout(
    odin::u32 rows, odin::u32 columns);

}
