#include "munin/list.hpp"

#include <boost/make_unique.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>

namespace munin {

// ==========================================================================
// LIST::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct list::impl
{
    explicit impl(list &self) : self(self)
    {
    }

    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent get_preferred_size() const
    {
        using boost::adaptors::transformed;

        auto const &string_size = [](terminalpp::string const &item) {
            return item.size();
        };

        auto const preferred_width = terminalpp::coordinate_type(
            items.empty()
                ? 0
                : *boost::max_element(items | transformed(string_size)));

        auto const preferred_height =
            static_cast<terminalpp::coordinate_type>(items.size());

        return {preferred_width, preferred_height};
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    [[nodiscard]] terminalpp::point get_cursor_position() const
    {
        return selected_item_index ? terminalpp::point(0, *selected_item_index)
                                   : terminalpp::point(0, 0);
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(
        render_surface &surface, terminalpp::rectangle const &region) const
    {
        terminalpp::for_each_in_region(
            surface,
            region,
            [this](auto &elem, auto const column, auto const row) {
                elem = row < items.size() && column < items[row].size()
                         ? items[row][column]
                         : ' ';

                elem.attribute_.polarity_ =
                    selected_item_index && *selected_item_index == row
                        ? terminalpp::graphics::polarity::negative
                        : terminalpp::graphics::polarity::positive;
            });
    }

    // ======================================================================
    // HANDLE_MOUSE_REPORT
    // ======================================================================
    void handle_mouse_report(terminalpp::mouse::event const &ev)
    {
        if (auto const clicked_row = ev.position_.y_;
            clicked_row < items.size())
        {
            self.select_item(clicked_row);
        }
        else
        {
            self.select_item(std::nullopt);
        }
    }

    // ======================================================================
    // HANDLE_CURSOR_UP
    // ======================================================================
    void handle_cursor_up()
    {
        if (!items.empty())
        {
            if (auto const current_item = self.get_selected_item_index();
                current_item.has_value())
            {
                self.select_item(
                    *current_item == 0 ? std::nullopt
                                       : std::optional<int>(*current_item - 1));
            }
            else
            {
                self.select_item(
                    std::optional(static_cast<int>(items.size() - 1)));
            }
        }
    }

    // ======================================================================
    // HANDLE_CURSOR_DOWN
    // ======================================================================
    void handle_cursor_down()
    {
        if (!items.empty())
        {
            if (auto const current_item = self.get_selected_item_index();
                current_item.has_value())
            {
                self.select_item(
                    *current_item == items.size() - 1
                        ? std::nullopt
                        : std::optional<int>(*current_item + 1));
            }
            else
            {
                self.select_item(0);
            }
        }
    }

    // ======================================================================
    // HANDLE_KEYPRESS
    // ======================================================================
    void handle_keypress(terminalpp::virtual_key const &vk)
    {
        switch (vk.key)  // NOLINT
        {
            case terminalpp::vk::cursor_up:
                handle_cursor_up();
                break;

            case terminalpp::vk::cursor_down:
                handle_cursor_down();
                break;
        }
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void event(boost::any const &ev)
    {
        if (auto const *mouse_report =
                boost::any_cast<terminalpp::mouse::event>(&ev);
            mouse_report != nullptr)
        {
            handle_mouse_report(*mouse_report);
        }
        else if (auto const *keypress =
                     boost::any_cast<terminalpp::virtual_key>(&ev);
                 keypress != nullptr)
        {
            handle_keypress(*keypress);
        }
    }

    list &self;
    std::vector<terminalpp::string> items;
    std::optional<int> selected_item_index;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
list::list() : pimpl_{boost::make_unique<impl>(*this)}
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
list::~list() = default;

// ==========================================================================
// GET_SELECTED_ITEM_INDEX
// ==========================================================================
std::optional<int> list::get_selected_item_index() const
{
    return pimpl_->selected_item_index;
}

// ==========================================================================
// SELECT_ITEM
// ==========================================================================
void list::select_item(std::optional<int> const &index)
{
    // If the index is initialized, then it must refer to an actual
    // item in the list, otherwise it is a programming error.
    assert(!index.has_value() || index >= 0);
    assert(
        !index.has_value() || index < static_cast<int>(pimpl_->items.size()));

    pimpl_->selected_item_index = index;
    on_item_changed();
    on_cursor_position_changed();
    on_redraw({
        {{}, get_size()}
    });
}

// ==========================================================================
// SET_ITEMS
// ==========================================================================
void list::set_items(std::vector<terminalpp::string> const &items)
{
    pimpl_->items = items;
    pimpl_->selected_item_index =
        !pimpl_->selected_item_index.has_value() || pimpl_->items.empty()
            ? std::nullopt
        : *pimpl_->selected_item_index < pimpl_->items.size()  // NOLINT
            ? pimpl_->selected_item_index
            : std::optional(static_cast<int>(pimpl_->items.size() - 1));

    on_item_changed();
    on_cursor_position_changed();
    on_preferred_size_changed();
    on_redraw({
        {{}, get_size()}
    });
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent list::do_get_preferred_size() const
{
    return pimpl_->get_preferred_size();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point list::do_get_cursor_position() const
{
    return pimpl_->get_cursor_position();
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void list::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void list::do_event(boost::any const &ev)
{
    pimpl_->event(ev);
}

// ==========================================================================
// MAKE_LIST
// ==========================================================================
std::shared_ptr<list> make_list()
{
    return std::make_shared<list>();
}

}  // namespace munin