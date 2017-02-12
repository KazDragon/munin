#pragma once

#include "munin/text/document.hpp"

namespace munin { namespace text {

//* =========================================================================
/// \brief Provides a document model for a single-lined text control.
//* =========================================================================
class default_singleline_document
    : public munin::text::document
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    default_singleline_document();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~default_singleline_document();

private :
    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the document in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_size(terminalpp::extent size) override;

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to retrieve the size of the document in a
    /// custom manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_size() const override;

    //* =====================================================================
    /// \brief Called by set_caret_position().  Derived classes must
    /// override this function in order to set the caret's position in a
    /// custom manner.
    //* =====================================================================
    virtual void do_set_caret_position(terminalpp::point const& pt) override;

    //* =====================================================================
    /// \brief Called by get_caret_position().  Derived classes must
    /// override this function in order to retrieve the caret's position in a
    /// custom manner.
    //* =====================================================================
    virtual terminalpp::point do_get_caret_position() const override;

    //* =====================================================================
    /// \brief Called by set_caret_index().  Derived classes must
    /// override this function in order to set the caret's index in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_caret_index(odin::u32 index) override;

    //* =====================================================================
    /// \brief Called by get_caret_index().  Derived classes must override
    /// this function in order to retrieve the caret's position in a custom
    /// manner.
    //* =====================================================================
    virtual odin::u32 do_get_caret_index() const override;

    //* =====================================================================
    /// \brief Called by get_text_size().  Derived classes must override
    /// this function in order to get the size of the text in a custom
    /// manner.
    //* =====================================================================
    virtual odin::u32 do_get_text_size() const override;

    //* =====================================================================
    /// \brief Called by insert_text().  Derived classes must override this
    /// function in order to insert text into the document in a custom
    /// manner.
    //* =====================================================================
    virtual void do_insert_text(
        terminalpp::string const  &text
      , boost::optional<odin::u32> index) override;

    //* =====================================================================
    /// \brief Called by delete_text().  Derived classes must override this
    /// function in order to delete text in a custom manner.
    //* =====================================================================
    virtual void do_delete_text(std::pair<odin::u32, odin::u32> range) override;

    //* =====================================================================
    /// \brief Called by set_text().  Derived classes must override this
    /// function in order to set text in a custom manner.
    //* =====================================================================
    virtual void do_set_text(terminalpp::string const &text) override;

    //* =====================================================================
    /// \brief Called by get_number_of_lines().  Derived classes must
    /// override this function in order to get the number of lines in the
    /// document in a custom manner.
    //* =====================================================================
    virtual odin::u32 do_get_number_of_lines() const override;

    //* =====================================================================
    /// \brief Called by get_line().  Derived classes must override this
    /// function in order to return the text line in a custom manner.
    //* =====================================================================
    virtual terminalpp::string do_get_line(odin::u32 index) const override;

    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}}
