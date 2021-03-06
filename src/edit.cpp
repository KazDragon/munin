#include "munin/edit.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>
#include <boost/make_unique.hpp>
#include <boost/range/adaptor/filtered.hpp>

namespace munin {

// ==========================================================================
// EDIT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct edit::impl
{
    edit &self_;
    terminalpp::string content;
    terminalpp::point cursor_position{0, 0};

    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(edit &self)
      : self_(self)
    {
    }

    // ======================================================================
    // INSERT_TEXT
    // ======================================================================
    void insert_text(terminalpp::string const &text)
    {
        using std::begin;
        using std::end;
        
        auto const &is_visible_in_edits = 
            [&](auto const &element)
            {
                auto const &is_control_element = 
                    [](auto const &element)
                    {
                        return element.glyph_.character_ 
                            <= terminalpp::detail::ascii::esc;
                    };
            
                return is_printable(element.glyph_)
                    && !is_control_element(element);
            };
            
        auto const old_content_size = 
            terminalpp::coordinate_type(content.size());
        
        auto const insertable_text = 
            text | boost::adaptors::filtered(is_visible_in_edits);
            
        auto const old_cursor_position = cursor_position;
        
        content.insert(
            content.begin() + cursor_position.x_,
            begin(insertable_text),
            end(insertable_text));
        
        auto const new_content_size = terminalpp::coordinate_type(content.size());
        auto const added_content_size = new_content_size - old_content_size;

        self_.set_cursor_position({
            cursor_position.x_ + added_content_size,
            cursor_position.y_
        });
        
        // Adding new text causes not only the space under the old cursor to
        // be redrawn, but also everything to the right of that as it is shifted
        // along one character.

        // This must then be trimmed to the extents of the space currently taken 
        // by the edit.
        terminalpp::coordinate_type const remaining_space = std::max(
            terminalpp::coordinate_type(self_.get_size().width_ - old_cursor_position.x_),
            terminalpp::coordinate_type(0));

        terminalpp::coordinate_type const changed_text_length = std::min(
            new_content_size - old_cursor_position.x_,
            remaining_space);
            
        self_.on_preferred_size_changed();
        
        self_.on_redraw({
            {{old_cursor_position.x_, 0}, {changed_text_length, 1}}
        });
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void key_event(terminalpp::virtual_key const &vk)
    {
        switch(vk.key)
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
                handle_text(char(vk.key));
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
            auto const current_cursor_position = self_.get_cursor_position();
            
            auto const new_cursor_x = std::min(
                terminalpp::coordinate_type(content.size()),
                mouse.position_.x_);
            
            self_.set_cursor_position({
                new_cursor_x, 
                current_cursor_position.y_
            });

            self_.set_focus();
        }
    }
    
private:
    // ======================================================================
    // HANDLE_CURSOR_LEFT
    // ======================================================================
    void handle_cursor_left()
    {
        if (cursor_position.x_ != 0)
        {
            self_.set_cursor_position({
                cursor_position.x_ - 1,
                cursor_position.y_
            });
        }
    }

    // ======================================================================
    // HANDLE_CURSOR_RIGHT
    // ======================================================================
    void handle_cursor_right()
    {
        if (cursor_position.x_ < content.size())
        {
            self_.set_cursor_position({
                cursor_position.x_ + 1,
                cursor_position.y_
            });
        }
    }

    // ======================================================================
    // HANDLE_HOME
    // ======================================================================
    void handle_home()
    {
        self_.set_cursor_position({0, cursor_position.y_});
    }

    // ======================================================================
    // HANDLE_HOME
    // ======================================================================
    void handle_end()
    {
        auto const rightmost_cursor_position = 
            terminalpp::coordinate_type(content.size());
        self_.set_cursor_position({
            rightmost_cursor_position, cursor_position.y_});
    }

    // ======================================================================
    // HANDLE_BACKSPACE
    // ======================================================================
    void handle_backspace()
    {
        auto const cursor_position = self_.get_cursor_position();
        
        if (cursor_position.x_ != 0)
        {
            auto const redraw_amount = 
                (terminalpp::coordinate_type(content.size()) - cursor_position.x_) + 1;

            auto const erased_content = 
                content.begin() + (cursor_position.x_ - 1);
            content.erase(erased_content, erased_content + 1);
    
            self_.set_cursor_position({
                cursor_position.x_ - 1,
                cursor_position.y_
            });
            
            self_.on_redraw({{
                { cursor_position.x_ - 1, cursor_position.y_ },
                { redraw_amount, 1 }
            }});
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
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
edit::edit()
  : pimpl_(boost::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
edit::~edit() = default;

// ==========================================================================
// GET_TEXT
// ==========================================================================
terminalpp::string edit::get_text() const
{
    return pimpl_->content;
}

// ==========================================================================
// INSERT_TEXT
// ==========================================================================
void edit::insert_text(terminalpp::string const &text)
{
    pimpl_->insert_text(text);
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent edit::do_get_preferred_size() const
{
    return terminalpp::extent(
        terminalpp::coordinate_type(pimpl_->content.size() + 1),
        1);
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
    return pimpl_->cursor_position;
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
void edit::do_set_cursor_position(terminalpp::point const& position)
{
    pimpl_->cursor_position = position;
    on_cursor_position_changed();
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void edit::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    auto const size = get_size();
    
    terminalpp::for_each_in_region(
        surface,
        region,
        [=](terminalpp::element &elem,
            terminalpp::coordinate_type column,
            terminalpp::coordinate_type row)
        {
            if (column < pimpl_->content.size())
            {
                elem = pimpl_->content[column];
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
    auto *vk = boost::any_cast<terminalpp::virtual_key>(&ev);
    
    if (vk != nullptr)
    {
        pimpl_->key_event(*vk);
        return;
    }
    
    auto *mouse = boost::any_cast<terminalpp::mouse::event>(&ev);
    
    if (mouse != nullptr)
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
    
}
