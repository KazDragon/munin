#include "munin/text_area.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>
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
    // GET_PREFERRED_SIZE
    // ======================================================================
    auto get_preferred_size()
    {
        return terminalpp::extent{
            self_.get_size().width_,
            std::max(
                terminalpp::coordinate_type(1), 
                terminalpp::coordinate_type(laid_out_text_.size()))};
    }

    // ======================================================================
    // GET_LENGTH
    // ======================================================================
    auto get_length() const
    {
        return text_area::text_index(text_.size());
    }

    // ======================================================================
    // GET_CARET_POSITION
    // ======================================================================
    auto get_caret_position() const
    {
        return caret_position_;
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
    // GET_CURSOR_POSITION
    // ======================================================================
    auto get_cursor_position() const
    {
        return cursor_position_;
    }

    // ======================================================================
    // SET_CURSOR_POSITION
    // ======================================================================
    void set_cursor_position(terminalpp::point const &position)
    {
        cursor_position_ = clamp_cursor_position(position);
        update_caret_position();
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
    // GET_TEXT
    // ======================================================================
    auto get_text() const
    {
        return text_;
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(render_surface &surface, terminalpp::rectangle const &region)
    {
        terminalpp::for_each_in_region(
            surface, 
            region, 
            [this](terminalpp::element &elem,
                terminalpp::coordinate_type column,
                terminalpp::coordinate_type row)
            {
                elem = (laid_out_text_.size() > row
                     && laid_out_text_[row].size() > column)
                      ? laid_out_text_[row][column]
                      : ' ';
            });
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void event(boost::any const &ev)
    {
        auto const *mouse_event = 
            boost::any_cast<terminalpp::mouse::event>(&ev);

        if (mouse_event != nullptr)
        {
            handle_mouse_event(*mouse_event);
            return;
        }

        auto const *keypress_event =
            boost::any_cast<terminalpp::virtual_key>(&ev);

        if (keypress_event != nullptr)
        {
            handle_keypress_event(*keypress_event);
            return;
        }
    }

    // ======================================================================
    // RESIZE
    // ======================================================================
    void resize()
    {
        layout_text();
        update_cursor_position();
    }

private:
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

    // ======================================================================
    // CLAMP_CURSOR_POSITION
    // ======================================================================
    terminalpp::point clamp_cursor_position(terminalpp::point position)
    {
        // Fit the requested cursor position with the bounds of the laid
        // out text.
        position.y_ =
            boost::algorithm::clamp(
                position.y_, 
                0, 
                laid_out_text_.size() - 1);
        position.x_ =
            boost::algorithm::clamp(
                position.x_, 
                0, 
                std::min(
                    terminalpp::coordinate_type(self_.get_size().width_ - 1),
                    terminalpp::coordinate_type(
                        laid_out_text_[position.y_].size())));

        return position;
    }   

    // ======================================================================
    // UPDATE_CURSOR_POSITION
    // ======================================================================
    void update_cursor_position()
    {
        auto const text_area_width = self_.get_size().width_;
        auto last_newline_index = text_area::text_index{0};
        
        auto cursor_position = terminalpp::point{};
        bool wrap = false;

        for (text_area::text_index index = 0; index < caret_position_; ++index)
        {
            // If the character is a newline, then the cursor position only
            // advances if it is not at the very end of a line.  This is to
            // prevent it turning into a douindex != (caret_position - 1) &&ble newline in that circumstance.
            bool const wrapped = std::exchange(wrap, false);

            if (text_[index] == '\n')
            {
                if (!wrapped)
                {
                    cursor_position.x_ = 0;
                    ++cursor_position.y_;
                }
            }
            else if ((cursor_position.x_ + 1) == text_area_width)
            {
                cursor_position.x_ = 0;
                ++cursor_position.y_;
                wrap = true;
            }
            else
            {
                ++cursor_position.x_;
            }
        }

        cursor_position_ = cursor_position;
        self_.on_cursor_position_changed();
    }

    // ======================================================================
    // UPDATE_CARET_POSITION
    // ======================================================================
    void update_caret_position()
    {
        auto const &text_area_width = self_.get_size().width_;
        auto current_position = terminalpp::point{};
        auto wrap = false;
        caret_position_ = 0;

        while (current_position != cursor_position_)
        {
            bool const wrapped = std::exchange(wrap, false);

            if (text_[caret_position_] == '\n')
            {
                if (!wrapped)
                {
                    current_position.x_ = 0;
                    ++current_position.y_;
                }
            }
            else if ((current_position.x_ + 1) == text_area_width)
            {
                current_position.x_ = 0;
                ++current_position.y_;
                wrap = true;
            }
            else
            {
                ++current_position.x_;
            }
            
            ++caret_position_;
        }

        if (wrap
         && caret_position_ < text_.size()
         && text_[caret_position_] == '\n')
        {
            ++caret_position_;
        }
    }

    // ======================================================================
    // HANDLE_MOUSE_EVENT
    // ======================================================================
    void handle_mouse_event(terminalpp::mouse::event const &ev)
    {
        set_cursor_position(ev.position_);
    }

    // ======================================================================
    // HANDLE_KEYPRESS_EVENT
    // ======================================================================
    void handle_keypress_event(terminalpp::virtual_key const &ev)
    {
        switch (ev.key)
        {
            case terminalpp::vk::cursor_left:
                set_caret_position(caret_position_ - ev.repeat_count);
                break;
                
            case terminalpp::vk::cursor_right:
                set_caret_position(caret_position_ + ev.repeat_count);
                break;

            case terminalpp::vk::cursor_up:
                set_cursor_position({
                    cursor_position_.x_,
                    std::max(cursor_position_.y_ - ev.repeat_count, 0)});
                break;

            case terminalpp::vk::cursor_down:
                set_cursor_position({
                    cursor_position_.x_,
                    std::max(cursor_position_.y_ + ev.repeat_count, 0)});
                break;
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
    return pimpl_->get_caret_position();
}

// ==========================================================================
// SET_CARET_POSITION
// ==========================================================================
void text_area::set_caret_position(text_index position)
{
    pimpl_->set_caret_position(position);
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
// GET_TEXT
// ==========================================================================
terminalpp::string text_area::get_text() const
{
    return pimpl_->get_text();
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void text_area::do_set_size(terminalpp::extent const &size)
{
    auto const old_preferred_size = get_preferred_size();

    basic_component::do_set_size(size);
    pimpl_->resize();

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
    return pimpl_->get_preferred_size();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point text_area::do_get_cursor_position() const
{
    return pimpl_->get_cursor_position();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
void text_area::do_set_cursor_position(terminalpp::point const &position)
{
    pimpl_->set_cursor_position(position);
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
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void text_area::do_event(boost::any const &ev)
{
    pimpl_->event(ev);
}

// ==========================================================================
// MAKE_TEXT_AREA
// ==========================================================================
std::shared_ptr<text_area> make_text_area()
{
    return std::make_shared<text_area>();
}

}
