#pragma once

#include "munin/core.hpp"
#include <terminalpp/extent.hpp>
#include <gsl.h>
#include <boost/any.hpp>
#include <memory>

namespace munin {

class component;

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a specified manner.
//* =========================================================================
class MUNIN_EXPORT layout
{
public :
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~layout() = default;

    //* =====================================================================
    /// \brief Returns the preferred size of this layout.
    /// \par
    /// The preferred size of the layout is the size that the layout would be
    /// if all components in it were at their preferred sizes.
    //* =====================================================================
    terminalpp::extent get_preferred_size(
        gsl::span<std::shared_ptr<component>> const &components,
        gsl::span<boost::any>                 const &hints) const;

    //* =====================================================================
    /// \brief Performs a layout of the specified components within the
    /// specified bounds.
    //* =====================================================================
    void operator()(
        gsl::span<std::shared_ptr<component>> const &components,
        gsl::span<boost::any>                 const &hints,
        terminalpp::extent                           size);

protected :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    layout() = default;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size(
        gsl::span<std::shared_ptr<component>> const &components,
        gsl::span<boost::any>                 const &hints) const = 0;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        gsl::span<std::shared_ptr<component> > const &components,
        gsl::span<boost::any>                  const &hints,
        terminalpp::extent                            size) = 0;
};

}
