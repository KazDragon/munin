#include "munin/list.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <munin/render_surface.hpp>
#include <boost/make_unique.hpp>
#include <boost/optional.hpp>

namespace munin {

// ==========================================================================
// LIST::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct list::impl
{
    std::vector<terminalpp::string> items_;
    boost::optional<int> selected_item_index_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
list::list()
  : pimpl_{boost::make_unique<impl>()}
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
list::~list() = default;

// ==========================================================================
// GET_SELECTED_ITEM_INDEX
// ==========================================================================
boost::optional<int> list::get_selected_item_index() const
{
    return pimpl_->selected_item_index_;
}

// ==========================================================================
// SET_SELECTED_ITEM_INDEX
// ==========================================================================
void list::select_item(boost::optional<int> const &index)
{
    // If the index is initialized, then it must refer to an actual
    // item in the list, otherwise it is a programming error.
    assert(!index.is_initialized() || index >=0);
    assert(!index.is_initialized() || index < static_cast<int>(pimpl_->items_.size()));

    pimpl_->selected_item_index_ = index;
    on_item_changed();
}

// ==========================================================================
// SET_ITEMS
// ==========================================================================
void list::set_items(std::vector<terminalpp::string> const &items)
{
    pimpl_->items_ = items;
    on_preferred_size_changed();
    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent list::do_get_preferred_size() const
{
    return pimpl_->items_.empty()
         ? terminalpp::extent{}
         : terminalpp::extent(pimpl_->items_[0].size(), 1);
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void list::do_draw(
    render_surface &surface,
    terminalpp::rectangle const &region) const
{
    terminalpp::for_each_in_region(
        surface, 
        region,
        [this](auto &elem, auto const column, auto const row)
        {
            elem = row < pimpl_->items_.size()
                && column < pimpl_->items_[row].size()
                 ? pimpl_->items_[row][column]
                 : ' ';
                
            elem.attribute_.polarity_ =
                pimpl_->selected_item_index_
             && *pimpl_->selected_item_index_ == row
              ? terminalpp::ansi::graphics::polarity::negative
              : terminalpp::ansi::graphics::polarity::positive;
        });
}

// ==========================================================================
// MAKE_LIST
// ==========================================================================
std::shared_ptr<list> make_list()
{
    return std::make_shared<list>();
}

    
}