#include "munin/list.hpp"

namespace munin {

// ==========================================================================
// GET_SELECTED_ITEM_INDEX
// ==========================================================================
boost::optional<int> list::get_selected_item_index() const
{
    return {};
}

// ==========================================================================
// SET_SELECTED_ITEM_INDEX
// ==========================================================================
void list::set_selected_item_index(boost::optional<int> const &index)
{

}

// ==========================================================================
// SET_ITEMS
// ==========================================================================
void list::set_items(std::vector<terminalpp::string> const &items)
{
    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent list::do_get_preferred_size() const
{
    return {};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void list::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
}

// ==========================================================================
// MAKE_LIST
// ==========================================================================
std::shared_ptr<list> make_list()
{
    return std::make_shared<list>();
}

    
}