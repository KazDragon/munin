#include "munin/text_area.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <boost/algorithm/clamp.hpp>
#include <boost/make_unique.hpp>
#include <boost/range/algorithm_ext/insert.hpp>

namespace munin {

// ==========================================================================
// TEXT_AREA::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct text_area::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(text_area &self)
      : self_(self)
    {
    }

    // ======================================================================
    // GET_LENGTH
    // ======================================================================
    auto get_length() const
    {
        return text_area::text_index(text_.size());
    }

    // ======================================================================
    // SET_CARET_POSITION
    // ======================================================================
    void set_caret_position(text_area::text_index position)
    {
        caret_position_ = boost::algorithm::clamp(
            position,
            0,
            get_length());
        update_cursor_position();
    }

    // ======================================================================
    // UPDATE_CURSOR_POSITION
    // ======================================================================
    void update_cursor_position()
    {
        auto const text_area_width = self_.get_size().width_;
        auto last_newline_index = text_area::text_index{0};
        
        auto cursor_position = terminalpp::point{};

        for (text_area::text_index index = 0; index < caret_position_; ++index)
        {
            // If the character is a newline, then the cursor position only
            // advances if it is not at the very end of a line.  This is to
            // prevent it turning into a double newline in that circumstance.
            if (text_[index] == '\n')
            {
                auto line_length = index - last_newline_index;
                
                if (line_length != text_area_width)
                {
                    cursor_position.x_ = 0;
                    ++cursor_position.y_;
                }
                
                last_newline_index = index;
            }
            else
            {
                ++cursor_position.x_;
            }
            
            // Wrap the cursor if necessary.
            if (cursor_position.x_ >= text_area_width)
            {
                cursor_position.x_ = 0;
                ++cursor_position.y_;
            }
        }

        cursor_position_ = cursor_position;
        self_.on_cursor_position_changed();
    }

    // ======================================================================
    // INSERT_TEXT
    // ======================================================================
    void insert_text(terminalpp::string const &text)
    {
        auto const new_caret_position = text_area::text_index(
            get_length() + text.size());

        boost::insert(text_, text_.begin() + caret_position_, text);
        layout_text();

        set_caret_position(new_caret_position);

        self_.on_preferred_size_changed();
        self_.on_redraw({{{}, self_.get_size()}});
    }

    // ======================================================================
    // INSERT_TEXT
    // ======================================================================
    void insert_text(
        terminalpp::string const &text, 
        text_area::text_index position)
    {
        boost::insert(text_, text_.begin() + position, text);
        layout_text();

        self_.on_preferred_size_changed();
        self_.on_redraw({{{}, self_.get_size()}});
    }

    // ======================================================================
    // LAYOUT_TEXT
    // ======================================================================
    void layout_text()
    {
        laid_out_text_.clear();
        laid_out_text_.emplace_back();

        auto const text_area_width = self_.get_size().width_;

        bool wrapped = false;
        for(auto const &ch : text_)
        {
            if (ch.glyph_.character_ == '\n')
            {
                // If we just wrapped a line, then absorb this newline.
                if (!wrapped)
                {
                    laid_out_text_.emplace_back();
                }
            }
            else
            {
                laid_out_text_.back() += ch;
            }

            if (laid_out_text_.back().size() == text_area_width)
            {
                wrapped = true;
                laid_out_text_.emplace_back();
            }
            else
            {
                wrapped = false;
            }
        }
    }

    text_area &self_;
    terminalpp::string text_;
    std::vector<terminalpp::string> laid_out_text_;

    text_area::text_index caret_position_{0};
    terminalpp::point cursor_position_{0, 0};
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
text_area::text_area()
  : pimpl_(boost::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
text_area::~text_area() = default;

// ==========================================================================
// GET_CARET_POSITION
// ==========================================================================
text_area::text_index text_area::get_caret_position() const
{
    return pimpl_->caret_position_;
}

// ==========================================================================
// GET_LENGTH
// ==========================================================================
text_area::text_index text_area::get_length() const
{
    return pimpl_->get_length();
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void text_area::insert_text(terminalpp::string const &text)
{
    pimpl_->insert_text(text);
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void text_area::insert_text(
    terminalpp::string const &text,
    text_area::text_index position)
{
    pimpl_->insert_text(text, position);
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void text_area::do_set_size(terminalpp::extent const &size)
{
    auto const old_preferred_size = get_preferred_size();

    basic_component::do_set_size(size);
    pimpl_->layout_text();
    pimpl_->update_cursor_position();

    if (get_preferred_size() != old_preferred_size)
    {
        on_preferred_size_changed();
    }
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent text_area::do_get_preferred_size() const
{
    return terminalpp::extent{
        get_size().width_,
        std::max(
            terminalpp::coordinate_type(1), 
            terminalpp::coordinate_type(pimpl_->laid_out_text_.size()))};
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point text_area::do_get_cursor_position() const
{
    return pimpl_->cursor_position_;
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool text_area::do_get_cursor_state() const
{
    return true;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void text_area::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    terminalpp::for_each_in_region(
        surface, 
        region, 
        [this](terminalpp::element &elem,
               terminalpp::coordinate_type column,
               terminalpp::coordinate_type row)
        {
            elem = (pimpl_->laid_out_text_.size() > row
                && pimpl_->laid_out_text_[row].size() > column)
                 ? pimpl_->laid_out_text_[row][column]
                 : ' ';
        });
}

// ==========================================================================
// MAKE_TEXT_AREA
// ==========================================================================
std::shared_ptr<text_area> make_text_area()
{
    return std::make_shared<text_area>();
}

}
