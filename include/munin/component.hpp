#pragma once

#include "munin/export.hpp"
#include "munin/rectangle.hpp"
#include <terminalpp/canvas_view.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/point.hpp>
#include <json.hpp>
#include <boost/any.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>
#include <vector>

namespace munin {

//* =========================================================================
/// \brief An object capable of being drawn on a canvas.
/// \par
/// A component is a fundamental piece of the munin architecture and is the
/// base class for anything that is capable of being drawn in some way.
//* =========================================================================
class MUNIN_EXPORT component
{
public :
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~component() = default;

    //* =====================================================================
    /// \brief Sets the position of this component.  This does not cause a
    /// redraw, on the basis that the entity performing the move (usually
    /// a layout manager) knows about it, and is better informed about all
    /// regions redrawn.
    //* =====================================================================
    void set_position(terminalpp::point const &position);

    //* =====================================================================
    /// \brief Retrieve the position of this component.
    //* =====================================================================
    terminalpp::point get_position() const;

    //* =====================================================================
    /// \brief Sets the size of this component.  This does not cause a
    /// redraw, on the basis that the entity performing the resize (usually
    /// a layout manager) knows about it, and is better informed about all
    /// regions redrawn.  It does, however, inform an active layout to lay
    /// the components out.
    //* =====================================================================
    void set_size(terminalpp::extent const &size);

    //* =====================================================================
    /// \brief Retreives the size of this component.
    //* =====================================================================
    terminalpp::extent get_size() const;

    //* =====================================================================
    /// \brief Retrieves the preferred size of this component.
    /// \par
    /// The preferred size of a component is the size at which all elements
    /// can be displayed with clarity.  For example, the preferred size of
    /// a static text control with the following text:
    /// \code
    /// This is a
    /// Static Text Box
    /// \endcode
    /// would be (15, 2).
    //* =====================================================================
    terminalpp::extent get_preferred_size() const;

    //* =====================================================================
    /// \brief Returns whether this component currently has focus.
    //* =====================================================================
    bool has_focus() const;

    //* =====================================================================
    /// \brief Sets this component to have the focus.
    //* =====================================================================
    void set_focus();

    //* =====================================================================
    /// \brief Causes this component to lose its focus.
    //* =====================================================================
    void lose_focus();

    //* =====================================================================
    /// \brief If this component has subcomponents, causes the component to
    /// move the focus from the currently focused subcomponent to the next
    /// subcomponent.  If it is a leaf component, then it toggles its focus,
    /// simulating a container with one element.
    //* =====================================================================
    void focus_next();

    //* =====================================================================
    /// \brief If this component has subcomponents, causes the component to
    /// move the focus from the currently focused subcomponent to the
    /// previous subcomponent.  If it is a leaf component, then it toggles
    /// its focus, simulating a container with one element.
    //* =====================================================================
    void focus_previous();

    //* =====================================================================
    /// \brief Returns true if this component has a visible cursor, false
    /// otherwise.
    //* =====================================================================
    bool get_cursor_state() const;

    //* =====================================================================
    /// \brief Returns the cursor's current position within the component.
    //* =====================================================================
    terminalpp::point get_cursor_position() const;

    //* =====================================================================
    /// \brief Sets the cursor's current position within the component.
    //* =====================================================================
    void set_cursor_position(terminalpp::point const &position);

    //* =====================================================================
    /// \brief Draws the component.
    ///
    /// \param cvs the canvas on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void draw(
        terminalpp::canvas_view &cvs
      , rectangle const &region) const;

    //* =====================================================================
    /// \brief Send an event to the component.  This may be of any type.
    /// A component must specify the types of messages it may receive and
    /// what it will do with it.
    //* =====================================================================
    void event(boost::any const &event);

    //* =====================================================================
    /// \brief Returns details about the component in JSON format.
    //* =====================================================================
    nlohmann::json to_json() const;

    //* =====================================================================
    /// \fn on_redraw
    /// \param regions The regions of the component that requires redrawing.
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component should be redrawn.
    //* =====================================================================
    boost::signals2::signal
    <
        void (std::vector<rectangle> const &regions)
    > on_redraw;

    //* =====================================================================
    /// \fn on_preferred_size_changed
    /// \brief Certain components sizes want to change during their lifetime,
    /// such as text controls that grow with the text within them.  Connect
    /// to this signal in order to receive notifications about this.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_preferred_size_changed;

    //* =====================================================================
    /// \fn on_focus_set
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component has gained focus.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_focus_set;

    //* =====================================================================
    /// \fn on_focus_lost
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component has lost focus.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_focus_lost;

    //* =====================================================================
    /// \fn on_cursor_state_changed
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component's cursor state changes.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_cursor_state_changed;

    //* =====================================================================
    /// \fn on_cursor_position_changed
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component's cursor position changes.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_cursor_position_changed;

protected :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    component() = default;

    //* =====================================================================
    /// \brief Called by set_position().  Derived classes must override this
    /// function in order to set the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_position(terminalpp::point const &position) = 0;

    //* =====================================================================
    /// \brief Called by get_position().  Derived classes must override this
    /// function in order to get the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::point do_get_position() const = 0;

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_size(terminalpp::extent const &size) = 0;

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_size() const = 0;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const = 0;

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    virtual bool do_has_focus() const = 0;

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_focus() = 0;

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    virtual void do_lose_focus() = 0;

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_next() = 0;

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_previous() = 0;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    virtual bool do_get_cursor_state() const = 0;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual terminalpp::point do_get_cursor_position() const = 0;

    //* =====================================================================
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual void do_set_cursor_position(terminalpp::point const &position) = 0;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param cvs the canvas on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    virtual void do_draw(
        terminalpp::canvas_view &cvs,
        rectangle const &region) const = 0;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event) = 0;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    virtual nlohmann::json do_to_json() const = 0;
};

}
