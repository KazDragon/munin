#pragma once

#include "munin/basic_component.hpp"

#include <terminalpp/string.hpp>

#include <vector>

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a repeating pattern.
//* ================================ =========================================
class MUNIN_EXPORT brush : public munin::basic_component
{
public:
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
    explicit brush(terminalpp::string pattern);

    //* =====================================================================
    /// \brief Constructor
    /// Initialises the brush with the passed multi-line pattern.
    //* =====================================================================
    explicit brush(std::vector<terminalpp::string> pattern);

    //* =====================================================================
    /// \brief Sets the pattern to the default pattern (i.e. whitespace)
    //* =====================================================================
    void set_pattern();

    //* =====================================================================
    /// \brief Sets the pattern to the given one-line pattern
    //* =====================================================================
    void set_pattern(terminalpp::string const &pattern);

    //* =====================================================================
    /// \brief Sets the pattern to the given multi-line pattern
    //* =====================================================================
    void set_pattern(std::vector<terminalpp::string> const &pattern);

protected:
    //* =====================================================================
    /// \brief Returns true if it is allowed for the component to receive
    /// focus, false otherwise.  This is used by the focus functions of
    /// basic_component.  By default, a component may receive focus at all
    /// times.  Override this function to specify different behaviour in your
    /// component.
    //* =====================================================================
    [[nodiscard]] bool do_can_receive_focus() const override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed canvas.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(render_surface &surface, terminalpp::rectangle const &region)
        const override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] nlohmann::json do_to_json() const override;

private:
    std::vector<terminalpp::string> pattern_;
};

//* =========================================================================
/// \brief Returns a newly created brush with the default pattern.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<brush> make_brush();

//* =========================================================================
/// \brief Returns a newly created brush with the specified single-line
/// pattern.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<brush> make_brush(terminalpp::string pattern);

//* =========================================================================
/// \brief Returns a newly created brush with the specified multi-line
/// pattern.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<brush> make_brush(std::vector<terminalpp::string> pattern);

}  // namespace munin
