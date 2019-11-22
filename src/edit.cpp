#include "munin/edit.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
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
    impl(edit &self)
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
        
        auto const &is_control_element = 
            [](auto const &element)
            {
                return element.glyph_.character_ <= terminalpp::detail::ascii::ESC;
            };
            
        auto const &is_printable_element =
            [](auto const &element)
            {
                return is_printable(element.glyph_);
            };

        auto const &is_visible_in_edits = 
            [&](auto const &element)
            {
                return is_printable_element(element)
                    && !is_control_element(element);
            };
            
        terminalpp::coordinate_type const width = content.size();
        
        auto const insertable_text = 
            text | boost::adaptors::filtered(is_visible_in_edits);
            
        auto const old_cursor_position = cursor_position;
        
        content.insert(
            content.begin() + cursor_position.x,
            begin(insertable_text),
            end(insertable_text));
        
        terminalpp::coordinate_type const added_width = content.size() - width;
        self_.set_cursor_position({
            cursor_position.x + added_width,
            cursor_position.y
        });
        
        // Adding new text causes not only the space under the old cursor to
        // be redrawn, but also everything to the right of that as it is shifted
        // along one character.
        terminalpp::coordinate_type const changed_text_length = 
            content.size() - old_cursor_position.x;
        
        self_.on_preferred_size_changed();
        self_.on_redraw({
            {{old_cursor_position.x, 0}, {changed_text_length, 1}}
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
                if (cursor_position.x != 0)
                {
                    self_.set_cursor_position({
                        cursor_position.x - 1,
                        cursor_position.y
                    });
                }
                break;

            case terminalpp::vk::cursor_right:
                if (cursor_position.x < content.size())
                {
                    self_.set_cursor_position({
                        cursor_position.x + 1,
                        cursor_position.y
                    });
                }
                break;    

            default:
            {
                terminalpp::string text;
                text += char(vk.key);
                
                insert_text(text);
                break;
            }
        }
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
    return terminalpp::extent(pimpl_->content.size() + 1, 1);
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
    terminalpp::for_each_in_region(
        surface,
        region,
        [this](terminalpp::element &elem,
               terminalpp::coordinate_type column,
               terminalpp::coordinate_type row)
        {
            if (column < pimpl_->content.size())
            {
                elem = pimpl_->content[column];
            }
            else
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
