#pragma once

#include "munin/layout.hpp"
#include <terminalpp/extent.hpp>
#include <boost/shared_ptr.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a grid of cells of equal size.
///
/// +------------+
/// |+--++--++--+|
/// ||  ||  ||  ||
/// |+--++--++--+|
/// |+--++--++--+|
/// ||  ||  ||  ||  3x3 grid
/// |+--++--++--+|
/// |+--++--++--+|
/// ||  ||  ||  ||
/// |+--++--++--+|
/// +------------+
//* =========================================================================
class MUNIN_EXPORT grid_layout final
    : public layout
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit grid_layout(terminalpp::extent dimensions);

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints,
        terminalpp::extent                             size) const override;
        
private :
    terminalpp::extent dimensions_;
};

//* =========================================================================
/// \brief Returns a newly created horizontal strip layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_grid_layout(terminalpp::extent size);

}