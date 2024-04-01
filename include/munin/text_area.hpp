#pragma once

#include "munin/basic_component.hpp"

#include <terminalpp/string.hpp>

#include <memory>

namespace munin {

//* =========================================================================
/// A component that represents a text area.
//* =========================================================================
class MUNIN_EXPORT text_area : public basic_component  // NOLINT
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
    /// \brief Returns the position of the caret.
    //* =====================================================================
    [[nodiscard]] text_index get_caret_position() const;

    //* =====================================================================
    /// \brief Sets the position of the caret.
    //* =====================================================================
    void set_caret_position(text_index position);

    //* =====================================================================
    /// \brief Returns the length of the document.
    ///
    /// The maximum length of the document is the maximum index at which the
    /// caret can be placed.
    //* =====================================================================
    [[nodiscard]] text_index get_length() const;

    //* =====================================================================
    /// \brief Inserts text at the current caret position, after which the
    /// caret will be at the end of the inserted text.
    //* =====================================================================
    void insert_text(terminalpp::string const &text);

    //* =====================================================================
    /// \brief Inserts text at the specified caret position, during which the
    /// caret will not move.
    //* =====================================================================
    void insert_text(terminalpp::string const &text, text_index position);

    //* =====================================================================
    /// \brief Returns the contents of the text area.
    //* =====================================================================
    [[nodiscard]] terminalpp::string get_text() const;

protected:
    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size() const override;

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
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    [[nodiscard]] bool do_get_cursor_state() const override;

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
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created empty text area.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<text_area> make_text_area();

}  // namespace munin