#pragma once

#include "munin/basic_component.hpp"
#include <terminalpp/string.hpp>
#include <memory>

namespace munin {

//* =========================================================================
/// A component that represents a text area.
//* =========================================================================
class MUNIN_EXPORT text_area : public basic_component
{
public:
    // A type that represents a uniquely indexed position within the document.
    using text_index = std::int32_t;

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    text_area();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~text_area() override;

    //* =====================================================================
    /// \Brief Returns the position of the caret.
    //* =====================================================================
    text_index get_caret_position() const;

    //* =====================================================================
    /// \brief Returns the length of the document.
    ///
    /// The maximum length of the document is the maximum index at which the
    /// caret can be placed.
    //* =====================================================================
    text_index get_length() const;

    //* =====================================================================
    /// \brief Inserts text at the current caret position, after which the
    /// caret will be at the end of the inserted text.
    //* =====================================================================
    void insert_text(terminalpp::string const &text);

    //* =====================================================================
    /// \brief Inserts text at the specified caret position, during which the
    /// caret will not move. 
    //* =====================================================================
    void insert_text(
        terminalpp::string const &text, 
        text_index position);

    //* =====================================================================
    /// \fn on_caret_position_changed
    /// \brief Connect to this signal in order to receive notifications about
    /// when the component's caret position changes.
    //* =====================================================================
    boost::signals2::signal
    <
        void ()
    > on_caret_position_changed;

protected:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    terminalpp::point do_get_cursor_position() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    bool do_get_cursor_state() const override;

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

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created empty text area.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<text_area> make_text_area();

}