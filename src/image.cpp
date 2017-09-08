#include "munin/image.hpp"
#include "munin/context.hpp"
#include <terminalpp/canvas_view.hpp>
#include <algorithm>
#include <utility>

using namespace terminalpp::literals;

namespace munin {

// ==========================================================================
// BRUSH::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct image::impl
{
    std::vector<terminalpp::string> content_;
    //terminalpp::element fill_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::element fill)
  : pimpl_(std::make_shared<impl>())
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::string content, terminalpp::element fill)
  : image(std::vector<terminalpp::string>{content}, fill)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(
    std::vector<terminalpp::string> content, 
    terminalpp::element fill)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->content_ = content;
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
image::~image()
{
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent image::do_get_preferred_size() const
{
    return pimpl_->content_.empty()
         ? terminalpp::extent()
         : terminalpp::extent(
               std::max_element(
                   pimpl_->content_.begin(),
                   pimpl_->content_.end(),
                   [](auto const &lhs, auto const &rhs)
                   {
                       return lhs.size() < rhs.size();
                   })->size(),
               pimpl_->content_.size());
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void image::do_draw(context &ctx, rectangle const &region) const
{
    /*
    auto &cvs = ctx.get_canvas();

    for (terminalpp::coordinate_type row = region.origin.y;
         row < region.origin.y + region.size.height;
         ++row)
    {
        auto const fill_row = row % pimpl_->content_.size();

        for (terminalpp::coordinate_type column = region.origin.x;
             column < region.origin.x + region.size.width;
             ++column)
        {
            auto const fill_column = column % pimpl_->content_[fill_row].size();
            
            cvs[column][row] = pimpl_->content_[fill_row][fill_column];
        }
    }
    */
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json image::do_to_json() const
{
    /*
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "image" }
    ])"_json;

    auto json = basic_component::do_to_json().patch(patch);
    
    json["content"]["size"] = pimpl_->content_.size();
    
    for (size_t index = 0; index < pimpl_->content_.size(); ++index)
    {
        json["content"]["content"][index] = 
            terminalpp::to_string(pimpl_->content_[index]);
    }
    
    return json;
    */
    return basic_component::do_to_json();
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
std::shared_ptr<image> make_image()
{
    return std::make_shared<image>();
}

}

