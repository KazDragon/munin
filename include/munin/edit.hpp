#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/string.hpp>

namespace munin {

//* =========================================================================
/// A component that represents a horizontally-scrolling editable
/// text area.
//* =========================================================================
class MUNIN_EXPORT edit : public basic_component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    edit();
    
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~edit() override;
    
    //* =====================================================================
    /// \brief Inserts the given text at the current cursor position, moving
    /// the cursor with the text.
    //* =====================================================================
    void insert_text(terminalpp::string const& text);
    
private:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    bool do_get_cursor_state() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    terminalpp::point do_get_cursor_position() const override;

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
        terminalpp::rectangle const &region) const override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created empty edit box.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<edit> make_edit();
    
}

