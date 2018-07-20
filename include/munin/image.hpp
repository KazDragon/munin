#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/string.hpp>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always filled with a centred
/// pattern.
//* =========================================================================
class MUNIN_EXPORT image : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    /// By default, the image shows only the background fill.
    //* =====================================================================
    explicit image(terminalpp::element fill = ' ');

    //* =====================================================================
    /// \brief Constructor
    /// Initialises the image with the passed single-line content.
    //* =====================================================================
    explicit image(
        terminalpp::string content,
        terminalpp::element fill = ' ');

    //* =====================================================================
    /// \brief Constructor
    /// Initialises the image with the passed multi-line content.
    //* =====================================================================
    explicit image(
        std::vector<terminalpp::string> content,
        terminalpp::element fill = ' ');

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~image() override;

    //* =====================================================================
    /// \brief Sets the background fill of the content.
    //* =====================================================================
    void set_fill(terminalpp::element const &fill);

    //* =====================================================================
    /// \brief Sets the content to the default content (i.e. fill only).
    //* =====================================================================
    void set_content();

    //* =====================================================================
    /// \brief Sets the content to the given one-line content.
    //* =====================================================================
    void set_content(terminalpp::string const &content);

    //* =====================================================================
    /// \brief Sets the content to the given multi-line content.
    //* =====================================================================
    void set_content(std::vector<terminalpp::string> const &content);

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
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(
        render_surface &surface,
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
/// \brief Returns a newly created image with the default content and,
/// optionally, a fill character.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<image> make_image(terminalpp::element fill = ' ');

//* =========================================================================
/// \brief Returns a newly created image with the specified single-line
/// content and, optionally, a fill character.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<image> make_image(
    terminalpp::string content,
    terminalpp::element fill = ' ');

//* =========================================================================
/// \brief Returns a newly created image with the specified multi-line
/// content and, optionally, a fill character.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<image> make_image(
    std::vector<terminalpp::string> content,
    terminalpp::element fill = ' ');

}
