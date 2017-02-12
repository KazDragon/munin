#pragma once

#include "munin/layout.hpp"

namespace munin {

BOOST_STATIC_CONSTANT(odin::u32, COMPASS_LAYOUT_CENTRE = 0);
BOOST_STATIC_CONSTANT(odin::u32, COMPASS_LAYOUT_NORTH  = 1);
BOOST_STATIC_CONSTANT(odin::u32, COMPASS_LAYOUT_EAST   = 2);
BOOST_STATIC_CONSTANT(odin::u32, COMPASS_LAYOUT_SOUTH  = 3);
BOOST_STATIC_CONSTANT(odin::u32, COMPASS_LAYOUT_WEST   = 4);

//* =========================================================================
/// \brief A class that can lay out its components according to basic compass
/// directions.  Components can be laid out to the north, east, south, west,
/// and centre.  Components to the north and south are given their
/// preferred height while having the width of the containing component, and
/// components to the west and east are given their preferred width, while
/// having the height of the containing component.
//* =========================================================================
class MUNIN_EXPORT compass_layout : public layout
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        std::vector<std::shared_ptr<component>> const &components
      , std::vector<boost::any>                 const &hints
      , terminalpp::extent                             size) override;
};

//* =========================================================================
/// \brief Returns a newly created compass layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_compass_layout();

}
