#pragma once

#include "munin/basic_component.hpp"
#include "munin/text/document.hpp"

namespace munin {

BOOST_STATIC_CONSTANT(
    std::string, EDIT_PASSWORD_ELEMENT = "Edit.Password.Element");

//* =========================================================================
/// \brief A class that models a single-line text control with a frame
/// bordering it.
//* =========================================================================
class MUNIN_EXPORT edit
    : public munin::basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    edit();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~edit();

    //* =====================================================================
    /// \brief Retrieves the document that this text area is using.
    //* =====================================================================
    std::shared_ptr<munin::text::document> get_document();

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    virtual bool do_get_cursor_state() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual terminalpp::point do_get_cursor_position() const override;

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
      , rectangle const &region) override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event) override;

    //* =====================================================================
    /// \brief Called by set_attribute().  Derived classes must override this
    /// function in order to set an attribute in a custom manner.
    //* =====================================================================
    virtual void do_set_attribute(
        std::string const &name, boost::any const &attr) override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created edit
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<edit> make_edit();

}
