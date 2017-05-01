#pragma once

#include "munin/layout.hpp"

namespace munin {

BOOST_STATIC_CONSTANT(odin::u32, HORIZONTAL_ALIGNMENT_LEFT   = 0);
BOOST_STATIC_CONSTANT(odin::u32, HORIZONTAL_ALIGNMENT_CENTRE = 1);
BOOST_STATIC_CONSTANT(odin::u32, HORIZONTAL_ALIGNMENT_RIGHT  = 2);

BOOST_STATIC_CONSTANT(odin::u32, VERTICAL_ALIGNMENT_TOP    = 0);
BOOST_STATIC_CONSTANT(odin::u32, VERTICAL_ALIGNMENT_CENTRE = 1);
BOOST_STATIC_CONSTANT(odin::u32, VERTICAL_ALIGNMENT_BOTTOM = 2);

struct alignment_data
{
    odin::u32 horizontal_alignment;
    odin::u32 vertical_alignment;
};

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a aligned-like manner.  Each cell of the aligned has an identical size.
/// Components added to the aligned will be displayed left-to-right, top-to-
/// bottom.
//* =========================================================================
class MUNIN_EXPORT aligned_layout : public layout
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
/// \brief Returns a newly created aligned layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_aligned_layout();

// Now a selection of alignment constants that you can use.

static alignment_data const alignment_hlvt = {
    HORIZONTAL_ALIGNMENT_LEFT,
    VERTICAL_ALIGNMENT_TOP
};

static alignment_data const alignment_hcvt = {
    HORIZONTAL_ALIGNMENT_CENTRE,
    VERTICAL_ALIGNMENT_TOP
};
static alignment_data const alignment_hrvt = {
    HORIZONTAL_ALIGNMENT_RIGHT,
    VERTICAL_ALIGNMENT_TOP
};

static alignment_data const alignment_hlvc = {
    HORIZONTAL_ALIGNMENT_LEFT,
    VERTICAL_ALIGNMENT_CENTRE
};

static alignment_data const alignment_hcvc = {
    HORIZONTAL_ALIGNMENT_CENTRE,
    VERTICAL_ALIGNMENT_CENTRE
};

static alignment_data const alignment_hrvc = {
    HORIZONTAL_ALIGNMENT_RIGHT,
    VERTICAL_ALIGNMENT_CENTRE
};

static alignment_data const alignment_hlvb = {
    HORIZONTAL_ALIGNMENT_LEFT,
    VERTICAL_ALIGNMENT_BOTTOM
};

static alignment_data const alignment_hcvb = {
    HORIZONTAL_ALIGNMENT_CENTRE,
    VERTICAL_ALIGNMENT_BOTTOM
};

static alignment_data const alignment_hrvb = {
    HORIZONTAL_ALIGNMENT_RIGHT,
    VERTICAL_ALIGNMENT_BOTTOM
};

}
