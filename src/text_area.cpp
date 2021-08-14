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
            width_,
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
    void resize(terminalpp::extent size)
    {
        width_ = size.width_;
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

        bool wrap = false;

        for(auto const &ch : text_)
        {
            bool const wrapped = std::exchange(wrap, false);

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

            if (laid_out_text_.back().size() == width_)
            {
                wrap = true;
                laid_out_text_.emplace_back();
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
                terminalpp::coordinate_type{0},
                terminalpp::coordinate_type(laid_out_text_.size() - 1));
        position.x_ =
            boost::algorithm::clamp(
                position.x_, 
                terminalpp::coordinate_type{0}, 
                std::min(
                    terminalpp::coordinate_type(width_ - 1),
                    terminalpp::coordinate_type(
                        laid_out_text_[position.y_].size())));

        return position;
    }   

    // ======================================================================
    // ADVANCE_CURSOR
    // ======================================================================
    auto advance_cursor(
        terminalpp::point cursor, 
        terminalpp::element const element_under_caret, 
        terminalpp::coordinate_type const width,
        bool &wrap)
    {
        auto const wrapped = std::exchange(wrap, false);

        if (element_under_caret == '\n')
        {
            if (!wrapped)
            {
                cursor.x_ = 0;
                ++cursor.y_;
            }
        }
        else if ((cursor.x_ + 1) == width)
        {
            cursor.x_ = 0;
            ++cursor.y_;
            wrap = true;
        }
        else
        {
            ++cursor.x_;
        }

        return cursor;
    }

    // ======================================================================
    // UPDATE_CURSOR_POSITION
    // ======================================================================
    void update_cursor_position()
    {
        cursor_position_ = {0, 0};
        bool wrapped = false;

        for (text_area::text_index index = 0; index < caret_position_; ++index)
        {
            cursor_position_ = advance_cursor(
                cursor_position_,
                text_[index],
                width_,
                wrapped);
        }

        self_.on_cursor_position_changed();
    }

    // ======================================================================
    // UPDATE_CARET_POSITION
    // ======================================================================
    void update_caret_position()
    {
        auto current_position = terminalpp::point{};
        auto wrapped = false;
        caret_position_ = 0;

        auto const &wrapped_newline_is_under_cursor =
            [&]()
            {
                return wrapped 
                    && caret_position_ < text_.size()
                    && text_[caret_position_] == '\n';
            };

        while (current_position != cursor_position_)
        {
            current_position = advance_cursor(
                current_position,
                text_[caret_position_],
                width_,
                wrapped);

            ++caret_position_;
        }

        if (wrapped_newline_is_under_cursor())
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
    terminalpp::coordinate_type width_{0};
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
    pimpl_->resize(size);

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
