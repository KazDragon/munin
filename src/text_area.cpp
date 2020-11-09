#include "munin/text_area.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <boost/make_unique.hpp>
#include <boost/range/algorithm/for_each.hpp>

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
    // LAYOUT_TEXT
    // ======================================================================
    void layout_text()
    {
        laid_out_text_.clear();
        laid_out_text_.emplace_back();

        auto const text_area_width = self_.get_size().width;

        for(auto const &ch : text_)
        {
            if (ch.glyph_.character_ == '\n')
            {
                laid_out_text_.emplace_back();
            }
            else
            {
                laid_out_text_.back() += ch;
            }
            
            if (laid_out_text_.back().size() == text_area_width)
            {
                laid_out_text_.emplace_back();
            }
        }
    }

    // ======================================================================
    // MOVE_CARET
    // ======================================================================
    void move_caret(text_area::text_index to_index)
    {
        auto const text_area_width = self_.get_size().width;
        auto last_newline_index = text_area::text_index{0};
        
        // For now, assume advance.
        for(; caret_position_ != to_index; ++caret_position_)
        {
            // If the character is a newline, then the cursor position only
            // advances if it is not at the very end of a line.  This is to
            // prevent it turning into a double newline in that circumstance.
            if (text_[caret_position_] == '\n')
            {
                auto line_length = caret_position_ - last_newline_index;
                
                if (line_length != text_area_width)
                {
                    cursor_position_.x = 0;
                    ++cursor_position_.y;
                }
                
                last_newline_index = caret_position_;
            }
            else
            {
                ++cursor_position_.x;
            }
            
            // Wrap the cursor if necessary.
            if (cursor_position_.x >= text_area_width)
            {
                cursor_position_.x = 0;
                ++cursor_position_.y;
            }
        }

        self_.on_caret_position_changed();
        self_.on_cursor_position_changed();
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
    return 0;
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void text_area::insert_text(terminalpp::string const &text)
{
    insert_text(text, pimpl_->caret_position_);
    pimpl_->move_caret(pimpl_->caret_position_ + text.size());
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void text_area::insert_text(
    terminalpp::string const &text,
    text_area::text_index position)
{
    pimpl_->text_.insert(
        pimpl_->text_.begin() + position, 
        text.begin(),
        text.end());

    on_preferred_size_changed();
    on_redraw({{{}, get_size()}});
    
    pimpl_->layout_text();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent text_area::do_get_preferred_size() const
{
    terminalpp::coordinate_type preferred_rows = 1;
    terminalpp::coordinate_type max_column_coordinate = 1;
    terminalpp::coordinate_type current_x_coordinate = 0;
    
    boost::for_each(
        pimpl_->text_,
        [&](auto const &elem)
        {
            if (elem.glyph_.character_ == '\n')
            {
                ++preferred_rows;
                current_x_coordinate = 0;
            }
            else
            {
                ++current_x_coordinate;
                max_column_coordinate = 
                    std::max(max_column_coordinate, current_x_coordinate);
            }
        });
    
    return {max_column_coordinate, preferred_rows};
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
