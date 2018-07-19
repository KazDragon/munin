#pragma once
#include <munin/component.hpp>
#include <munin/render_surface.hpp>
#include <gmock/gmock.h>

class mock_component : public munin::component
{
public :
    //* =====================================================================
    /// \brief Called by set_position().  Derived classes must override this
    /// function in order to set the position of the component in a custom
    /// manner.
    //* =====================================================================
    MOCK_METHOD1(do_set_position, void (terminalpp::point const &));

    //* =====================================================================
    /// \brief Called by get_position().  Derived classes must override this
    /// function in order to get the position of the component in a custom
    /// manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_get_position, terminalpp::point ());

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    MOCK_METHOD1(do_set_size, void (terminalpp::extent const &));

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_get_size, terminalpp::extent ());

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_get_preferred_size, terminalpp::extent ());

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_has_focus, bool ());

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    MOCK_METHOD0(do_set_focus, void ());

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    MOCK_METHOD0(do_lose_focus, void ());

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    MOCK_METHOD0(do_focus_next, void ());

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    MOCK_METHOD0(do_focus_previous, void ());

    //* =====================================================================
    /// \brief Called by enable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    MOCK_METHOD0(do_enable, void ());

    //* =====================================================================
    /// \brief Called by disable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    MOCK_METHOD0(do_disable, void ());

    //* =====================================================================
    /// \brief Called by is_enabled().  Derived classes must override this
    /// function in order to return whether the component is disabled or not
    /// in a custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_is_enabled, bool ());

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_get_cursor_state, bool ());

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_get_cursor_position, terminalpp::point ());

    //* =====================================================================
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    MOCK_METHOD1(do_set_cursor_position, void (terminalpp::point const &));

    //* =====================================================================
    /// \brief Called by set_attribute().  Derived classes must override this
    /// function in order to set an attribute in a custom manner.
    //* =====================================================================
    MOCK_METHOD2(
        do_set_attribute,
        void (std::string const &, boost::any const &));

    //* =====================================================================
    /// \brief Called by layout().  Derived classes must override this
    /// function in order to lay the component out.  If the component
    /// contains subcomponents, these must also be laid out.
    //* =====================================================================
    MOCK_METHOD0(do_layout, void ());

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed canvas.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param ctx the canvas on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    MOCK_CONST_METHOD2(do_draw, 
        void (munin::render_surface &, munin::rectangle const &));

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    MOCK_METHOD1(do_event, void (boost::any const &));

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    MOCK_CONST_METHOD0(do_to_json, nlohmann::json ());
};

//* =========================================================================
/// \brief Makes a mock component
//* =========================================================================
std::shared_ptr<mock_component> make_mock_component();
