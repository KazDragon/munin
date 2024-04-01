#pragma once

#include "munin/component.hpp"

namespace munin {

//* =========================================================================
/// \brief A default implementation of a component.  Only do_draw()
/// and do_get_preferred_size() remain unimplemented.
//* =========================================================================
class MUNIN_EXPORT basic_component : public component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    basic_component();

protected:
    //* =====================================================================
    /// \brief Returns true if it is allowed for the component to receive
    /// focus, false otherwise.
    //* =====================================================================
    [[nodiscard]] bool can_receive_focus() const;

    //* =====================================================================
    /// \brief Returns true if it is allowed for the component to receive
    /// focus, false otherwise.  This is used by the focus functions of
    /// basic_component.  By default, a component may receive focus at all
    /// times.  Override this function to specify different behaviour in your
    /// component.
    //* =====================================================================
    [[nodiscard]] virtual bool do_can_receive_focus() const;

    //* =====================================================================
    /// \brief Called by set_position().  Derived classes must override this
    /// function in order to set the position of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by get_position().  Derived classes must override this
    /// function in order to get the position of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::point do_get_position() const override;

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_size() const override;

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    [[nodiscard]] bool do_has_focus() const override;

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    void do_set_focus() override;

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    void do_lose_focus() override;

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    void do_focus_next() override;

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    void do_focus_previous() override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    [[nodiscard]] bool do_get_cursor_state() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::point do_get_cursor_position() const override;

    //* =====================================================================
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    void do_set_cursor_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] nlohmann::json do_to_json() const override;

private:
    terminalpp::rectangle bounds_;
    bool has_focus_;
};

}  // namespace munin
