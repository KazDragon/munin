#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/element.hpp>

namespace munin {

// Changing this boolean attribute to true will stop other attributes from
// changing until the attribute is once again false.
static const std::string ATTRIBUTE_LOCK = "Lock";

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a given element.  It allows changing the attribute and glyph of the
/// element independently.
//* =========================================================================
class MUNIN_EXPORT filled_box : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    filled_box(terminalpp::element const &element);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~filled_box();

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
    virtual terminalpp::extent do_get_preferred_size() const;

    //* =====================================================================
    /// \brief Called by set_attribute().  Derived classes must override this
    /// function in order to set an attribute in a custom manner.
    //* =====================================================================
    virtual void do_set_attribute(
        std::string const &name, boost::any const &attr);

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
