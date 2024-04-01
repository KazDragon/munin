#include "munin/edit.hpp"

#include "munin/render_surface.hpp"

#include <boost/algorithm/clamp.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

#include <memory>

namespace munin {

// ==========================================================================
// EDIT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct edit::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(edit &self) : self_(self)
    {
    }

    // ======================================================================
    // GET_CONTENT
    // ======================================================================
    [[nodiscard]] terminalpp::string const &get_content() const
    {
        return content_;
    }

    // ======================================================================
    // GET_LENGTH
    // ======================================================================
    [[nodiscard]] edit::text_index get_length() const
    {
        return static_cast<edit::text_index>(content_.size());
    }

    // ======================================================================
    // GET_CARET_POSITION
    // ======================================================================
    [[nodiscard]] edit::text_index get_caret_position() const
    {
        return caret_position_;
    }

    // ======================================================================
    // SET_CARET_POSITION
    // ======================================================================
    void set_caret_position(edit::text_index position)
    {
        caret_position_ = boost::algorithm::clamp(position, 0, get_length());
        update_cursor_position();
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    [[nodiscard]] terminalpp::point get_cursor_position() const
    {
        return cursor_position_;
    }

    // ======================================================================
    // UPDATE_CURSOR_POSITION
    // ======================================================================
    void update_cursor_position()
    {
        cursor_position_ = {
            boost::algorithm::clamp(
                caret_position_, 0, std::max(0, self_.get_size().width_ - 1)),
            0};
        self_.on_cursor_position_changed();
    }

    // ======================================================================
    // SET_TEXT
    // ======================================================================
    void set_text(terminalpp::string const &text)
    {
        auto const old_caret_position = caret_position_;

        content_ = text;

        auto const new_caret_position = std::min(
            old_caret_position, static_cast<text_index>(content_.size()));

        set_caret_position(new_caret_position);

        self_.on_preferred_size_changed();

        self_.on_redraw({
            {{0, 0}, self_.get_size()}
        });
    }

    // ======================================================================
    // INSERT_TEXT
    // ======================================================================
    void insert_text(terminalpp::string const &text)
    {
        using std::begin;
        using std::end;

        auto const &is_visible_in_edits = [&](auto const &element) {
            auto const &is_control_element = [](auto const &element) {
                return element.glyph_.character_
                    <= terminalpp::detail::ascii::esc;
            };

            return is_printable(element.glyph_) && !is_control_element(element);
        };

        auto const old_content_length = get_length();

        auto const insertable_text =
            text | boost::adaptors::filtered(is_visible_in_edits);

        auto const old_caret_position = caret_position_;

        content_.insert(
            content_.begin() + caret_position_,
            begin(insertable_text),
            end(insertable_text));

        auto const new_content_length = get_length();
        auto const added_content_length =
            new_content_length - old_content_length;

        set_caret_position(caret_position_ + added_content_length);

        // Adding new text causes not only the space under the old cursor to
        // be redrawn, but also everything to the right of that as it is shifted
        // along one character.

        // This must then be trimmed to the extents of the space currently taken
        // by the edit.
        auto const remaining_space =
            std::max(self_.get_size().width_ - old_caret_position, 0);

        auto const changed_text_length =
            std::min(new_content_length - old_caret_position, remaining_space);

        self_.on_preferred_size_changed();

        self_.on_redraw({
            {{old_caret_position, 0}, {changed_text_length, 1}}
        });
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void key_event(terminalpp::virtual_key const &vk)
    {
        switch (vk.key)
        {
            case terminalpp::vk::cursor_left:
                handle_cursor_left();
                break;

            case terminalpp::vk::cursor_right:
                handle_cursor_right();
                break;

            case terminalpp::vk::home:
                handle_home();
                break;

            case terminalpp::vk::end:
                handle_end();
                break;

            case terminalpp::vk::bs:
                // Fall-through
            case terminalpp::vk::del:
                handle_backspace();
                break;

            default:
                handle_text(static_cast<char>(vk.key));
                break;
        }
    }

    // ======================================================================
    // MOUSE_EVENT
    // ======================================================================
    void mouse_event(terminalpp::mouse::event const &mouse)
    {
        if (mouse.action_ == terminalpp::mouse::event_type::left_button_down)
        {
            set_caret_position(
                static_cast<edit::text_index>(mouse.position_.x_));
            self_.set_focus();
        }
    }

private:
    // ======================================================================
    // HANDLE_CURSOR_LEFT
    // ======================================================================
    void handle_cursor_left()
    {
        set_caret_position(caret_position_ - 1);
    }

    // ======================================================================
    // HANDLE_CURSOR_RIGHT
    // ======================================================================
    void handle_cursor_right()
    {
        set_caret_position(caret_position_ + 1);
    }

    // ======================================================================
    // HANDLE_HOME
    // ======================================================================
    void handle_home()
    {
        set_caret_position(0);
    }

    // ======================================================================
    // HANDLE_HOME
    // ======================================================================
    void handle_end()
    {
        set_caret_position(get_length());
    }

    // ======================================================================
    // HANDLE_BACKSPACE
    // ======================================================================
    void handle_backspace()
    {
        if (caret_position_ != 0)
        {
            auto const redraw_amount = (get_length() - caret_position_) + 1;

            auto const erased_content =
                content_.begin() + (caret_position_ - 1);
            content_.erase(erased_content, erased_content + 1);

            set_caret_position(caret_position_ - 1);
            self_.on_preferred_size_changed();

            self_.on_redraw({
                {{caret_position_, 0}, {redraw_amount, 1}}
            });
        }
    }

    // ======================================================================
    // HANDLE_TEXT
    // ======================================================================
    void handle_text(char ch)
    {
        terminalpp::string text;
        text += ch;

        insert_text(text);
    }

    edit &self_;
    terminalpp::string content_;
    edit::text_index caret_position_ = 0;
    terminalpp::point cursor_position_{0, 0};
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
edit::edit() : pimpl_(std::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
edit::~edit() = default;

// ==========================================================================
// SET_CARET_POSITION
// ==========================================================================
void edit::set_caret_position(edit::text_index position)
{
    pimpl_->set_caret_position(position);
}

// ==========================================================================
// GET_CARET_POSITION
// ==========================================================================
edit::text_index edit::get_caret_position() const
{
    return pimpl_->get_caret_position();
}

// ==========================================================================
// GET_LENGTH
// ==============s============================================================
edit::text_index edit::get_length() const
{
    return pimpl_->get_length();
}

// ==========================================================================
// GET_TEXT
// ==========================================================================
terminalpp::string edit::get_text() const
{
    return pimpl_->get_content();
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void edit::insert_text(terminalpp::string const &text)
{
    pimpl_->insert_text(text);
    on_cursor_position_changed();
}

// ==========================================================================
// SET_TEXT
// ==========================================================================
void edit::set_text(terminalpp::string const &content)
{
    pimpl_->set_text(content);
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void edit::do_set_size(terminalpp::extent const &size)
{
    basic_component::do_set_size(size);
    pimpl_->update_cursor_position();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent edit::do_get_preferred_size() const
{
    // An edit would prefer to have a width that is the length of the text
    // content, plus one space for a cursor to sit at the end.
    return terminalpp::extent{get_length() + 1, 1};
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool edit::do_get_cursor_state() const
{
    return true;
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point edit::do_get_cursor_position() const
{
    return pimpl_->get_cursor_position();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
void edit::do_set_cursor_position(terminalpp::point const &position)
{
    // For an edit, setting the cursor position is identical to setting
    // the caret to the same location.
    pimpl_->set_caret_position(position.x_);
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void edit::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
    auto const size = get_size();

    terminalpp::for_each_in_region(
        surface,
        region,
        [=](terminalpp::element &elem,
            terminalpp::coordinate_type column,  // NOLINT
            terminalpp::coordinate_type row) {
            if (column < get_length())
            {
                elem = pimpl_->get_content()[column];
            }
            else if (column < size.width_)
            {
                elem = ' ';
            }
        });
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void edit::do_event(boost::any const &ev)
{
    if (auto const *vk = boost::any_cast<terminalpp::virtual_key>(&ev);
        vk != nullptr)
    {
        pimpl_->key_event(*vk);
        return;
    }
    else if (auto const *mouse = boost::any_cast<terminalpp::mouse::event>(&ev);
             mouse != nullptr)
    {
        pimpl_->mouse_event(*mouse);
        return;
    }
}

// ==========================================================================
// MAKE_EDIT
// ==========================================================================
std::shared_ptr<edit> make_edit()
{
    return std::make_shared<edit>();
}

}  // namespace munin
