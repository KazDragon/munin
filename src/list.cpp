#include "munin/list.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/virtual_key.hpp>
#include <munin/render_surface.hpp>
#include <boost/make_unique.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/max_element.hpp>
namespace munin {

// ==========================================================================
// LIST::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct list::impl
{
    impl(list &self)
      : self_(self)
    {
    }

    // ======================================================================
    // HANDLE_MOUSE_REPORT
    // ======================================================================
    void handle_mouse_report(terminalpp::ansi::mouse::report const &report)
    {
        auto const clicked_row = report.y_position_;

        if (clicked_row < items_.size())
        {
            self_.select_item(clicked_row);
        }
        else
        {
            self_.select_item(boost::none);
        }
    }

    // ======================================================================
    // HANDLE_KEYPRESS
    // ======================================================================
    void handle_keypress(terminalpp::virtual_key const &vk)
    {
        if (items_.empty())
        {
            return;
        }

        auto const currently_selected_item_index = 
            self_.get_selected_item_index();

        switch (vk.key)
        {
            case terminalpp::vk::cursor_up:
                if (currently_selected_item_index)
                {
                    self_.select_item(
                        *currently_selected_item_index == 0
                      ? boost::none
                      : boost::optional<int>(*currently_selected_item_index - 1));
                }
                else
                {
                    self_.select_item(boost::optional<int>(items_.size() - 1));
                }
                break;

            case terminalpp::vk::cursor_down:
                if (currently_selected_item_index)
                {
                    self_.select_item(
                        *currently_selected_item_index == items_.size() - 1
                      ? boost::none
                      : boost::optional<int>(*currently_selected_item_index + 1));
                }
                else
                {
                    self_.select_item(0);
                }
                break;
        }
    }

    list &self_;
    std::vector<terminalpp::string> items_;
    boost::optional<int> selected_item_index_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
list::list()
  : pimpl_{boost::make_unique<impl>(*this)}
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
// SELECT_ITEM
// ==========================================================================
void list::select_item(boost::optional<int> const &index)
{
    // If the index is initialized, then it must refer to an actual
    // item in the list, otherwise it is a programming error.
    assert(!index.is_initialized() || index >=0);
    assert(!index.is_initialized() || index < static_cast<int>(pimpl_->items_.size()));

    pimpl_->selected_item_index_ = index;
    on_item_changed();
    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// SET_ITEMS
// ==========================================================================
void list::set_items(std::vector<terminalpp::string> const &items)
{
    pimpl_->items_ = items;
    pimpl_->selected_item_index_ = 
        !pimpl_->selected_item_index_.is_initialized()
     || pimpl_->items_.empty()
      ? boost::none
      : *pimpl_->selected_item_index_ < pimpl_->items_.size()
      ? pimpl_->selected_item_index_
      : boost::optional<int>(pimpl_->items_.size() - 1);
      
    on_item_changed();
    on_preferred_size_changed();
    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent list::do_get_preferred_size() const
{
    using boost::adaptors::transformed;

    auto const &string_size = 
        [](terminalpp::string const &item)
        {
            return item.size();
        };
    
    auto const preferred_width = 
        static_cast<terminalpp::coordinate_type>(
            pimpl_->items_.empty()
          ? 0
          : *boost::max_element(
                pimpl_->items_ | transformed(string_size)));

    auto const preferred_height = 
        static_cast<terminalpp::coordinate_type>(
            pimpl_->items_.size());

    return { preferred_width, preferred_height };
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
// DO_EVENT
// ==========================================================================
void list::do_event(boost::any const &ev)
{
    auto const *mouse_report = 
        boost::any_cast<terminalpp::ansi::mouse::report>(&ev);

    if (mouse_report != nullptr)
    {
        pimpl_->handle_mouse_report(*mouse_report);
    }

    auto const *keypress =
        boost::any_cast<terminalpp::virtual_key>(&ev);

    if (keypress != nullptr)
    {
        pimpl_->handle_keypress(*keypress);
    }
}

// ==========================================================================
// MAKE_LIST
// ==========================================================================
std::shared_ptr<list> make_list()
{
    return std::make_shared<list>();
}

    
}