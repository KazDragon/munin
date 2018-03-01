#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/element.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a given element.
//* =========================================================================
class MUNIN_EXPORT filled_box : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    filled_box(terminalpp::element const &element = ' ');

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~filled_box() override;

    //* =====================================================================
    /// \brief Sets the preferred size of this box.  The default is (1,1).
    //* =====================================================================
    void set_preferred_size(terminalpp::extent preferred_size);

    //* =====================================================================
    /// \brief Sets the element with which the box is filled.
    //* =====================================================================
    void set_fill(terminalpp::element const &element);

    //* =====================================================================
    /// \brief Gets the element with which the box is filled.
    //* =====================================================================
    terminalpp::element get_fill() const;

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed canvas.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param cvs the canvas in which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(
        terminalpp::canvas_view &cvs, 
        rectangle const &region) const override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created filled box.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<filled_box> make_fill(terminalpp::element const &fill);

}
