#pragma once

#include <munin/layout.hpp>
#include <gmock/gmock.h>

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a specified manner.
//* =========================================================================
class mock_layout : public munin::layout
{
public :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD2(
        do_get_preferred_size,
        terminalpp::extent (
            std::vector<std::shared_ptr<munin::component>> const &components,
            std::vector<boost::any>                        const &hints));

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    MOCK_CONST_METHOD3(
        do_layout,
        void (
            std::vector<std::shared_ptr<munin::component>> const &components,
            std::vector<boost::any>                        const &hints,
            terminalpp::extent                                    size));
};
