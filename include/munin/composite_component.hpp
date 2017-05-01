#pragma once

#include "munin/component.hpp"

namespace munin {

class container;

//* =========================================================================
/// \brief Implements a component that simply forwards from and to an
/// underlying container.
///
/// \par Purpose
/// Some components are implemented by composing together several pre-
/// existing components onto a container.  However, they should not be
/// exposed as a container, but as a component.  By inheriting from this
/// class, which simply forwards functions and messages to and from the
/// underlying container, this can be achieved more easily.
//* =========================================================================
class MUNIN_EXPORT composite_component
    : public component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    composite_component();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~composite_component();

protected :
    //* =====================================================================
    /// \brief Retrieve the underlying container.
    //* =====================================================================
    std::shared_ptr<container> get_container();

    //* =====================================================================
    /// \brief Called by set_position().  Derived classes must override this
    /// function in order to set the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by get_position().  Derived classes must override this
    /// function in order to get the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::point do_get_position() const override;

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_size() const override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    virtual bool do_has_focus() const override;

    //* =====================================================================
    /// \brief Called by set_can_focus().  Derived classes must override this
    /// function in order to set whether this component can be focussed in
    /// a custom manner.
    //* =====================================================================
    virtual void do_set_can_focus(bool focus) override;

    //* =====================================================================
    /// \brief Called by can_focus().  Derived classes must override this
    /// function in order to return whether this component can be focused in
    /// a custom manner.
    //* =====================================================================
    virtual bool do_can_focus() const override;

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_focus() override;

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    virtual void do_lose_focus() override;

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_next() override;

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_previous() override;

    //* =====================================================================
    /// \brief Called by get_focussed_component().  Derived classes must
    /// override this function in order to return the focussed component
    /// in a custom manner.
    //* =====================================================================
    virtual std::shared_ptr<component> do_get_focussed_component() override;

    //* =====================================================================
    /// \brief Called by enable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    virtual void do_enable() override;

    //* =====================================================================
    /// \brief Called by disable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    virtual void do_disable() override;

    //* =====================================================================
    /// \brief Called by is_enabled().  Derived classes must override this
    /// function in order to return whether the component is disabled or not
    /// in a custom manner.
    //* =====================================================================
    virtual bool do_is_enabled() const override;

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
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual void do_set_cursor_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by layout().  Derived classes must override this
    /// function in order to lay the component out.  If the component
    /// contains subcomponents, these must also be laid out.
    //* =====================================================================
    virtual void do_layout() override;

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

}
