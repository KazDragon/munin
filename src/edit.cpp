#include "munin/edit.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/virtual_key.hpp>
#include <boost/make_unique.hpp>

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
        content += text;
        cursor_position.x += text.size();
        
        self_.on_preferred_size_changed();
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void event(terminalpp::virtual_key const &vk)
    {
        terminalpp::string text;
        text += char(vk.key);
        
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
        pimpl_->event(*vk);
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
