#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/string.hpp>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a repeating pattern.
//* =========================================================================
class MUNIN_EXPORT brush : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    /// By default, a brush is filled with a repeating space element with
    /// default attributes.
    //* =====================================================================
    brush();

    //* =====================================================================
    /// \brief Constructor
    /// Initialises the brush with the passed single-line pattern.
    //* =====================================================================
    brush(terminalpp::string pattern);
    
    //* =====================================================================
    /// \brief Constructor
    /// Initialises the brush with the passed multi-line pattern.
    //* =====================================================================
    brush(std::vector<terminalpp::string> pattern);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~brush() override;

    //* =====================================================================
    /// \brief Sets the preferred size of this brush.  The default is (1,1).
    //* =====================================================================
    void set_preferred_size(terminalpp::extent preferred_size);

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param ctx the context in which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(context &ctx, rectangle const &region) const override;

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
/// \brief Returns a newly created brush with the default pattern.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<brush> make_brush();

}
