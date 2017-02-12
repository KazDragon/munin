#include "munin/image.hpp"
#include "munin/context.hpp"
#include "odin/core.hpp"
#include "terminalpp/canvas_view.hpp"
#include "terminalpp/string.hpp"
#include <algorithm>
#include <utility>

namespace munin {

// ==========================================================================
// IMAGE::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct image::impl : boost::noncopyable
{
    std::vector<terminalpp::string> elements_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(std::vector<terminalpp::string> const &elements)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->elements_ = elements;
    set_can_focus(false);
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::string const &elements)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->elements_ = {elements};
    set_can_focus(false);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
image::~image()
{
}

// ==========================================================================
// SET_IMAGE
// ==========================================================================
void image::set_image(std::vector<terminalpp::string> const &elements)
{
    pimpl_->elements_ = elements;
    on_preferred_size_changed();
    on_redraw({rectangle({}, get_size())});
}

// ==========================================================================
// SET_IMAGE
// ==========================================================================
void image::set_image(terminalpp::string const &element)
{
    set_image(std::vector<terminalpp::string>{element});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent image::do_get_preferred_size() const
{
    terminalpp::extent preferred_size;
    preferred_size.height = pimpl_->elements_.size();

    for (auto const &row : pimpl_->elements_)
    {
        preferred_size.width = (std::max)(
            odin::u32(preferred_size.width)
          , odin::u32(row.size()));
    }

    return preferred_size;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void image::do_draw(
    context         &ctx
  , rectangle const &region)
{
    static terminalpp::element const default_element(' ');
    auto &cvs = ctx.get_canvas();

    for (odin::u32 row = region.origin.y;
         row < odin::u32(region.origin.y + region.size.height);
         ++row)
    {
        for (odin::u32 column = region.origin.x;
             column < odin::u32(region.origin.x + region.size.width);
             ++column)
        {
            if (row < pimpl_->elements_.size()
             && column < pimpl_->elements_[row].size())
            {
                cvs[column][row] = pimpl_->elements_[row][column];
            }
            else
            {
                cvs[column][row] = default_element;
            }
        }
    }
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
MUNIN_EXPORT
std::shared_ptr<image> make_image(
    std::vector<terminalpp::string> const &elements)
{
    return std::make_shared<image>(elements);
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
MUNIN_EXPORT
std::shared_ptr<image> make_image(terminalpp::string const &elements)
{
    return std::make_shared<image>(elements);
}

}

