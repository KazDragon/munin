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
    terminalpp::element fill_;
};

// ==========================================================================
// DRAW_FILL_LINE
// ==========================================================================
static void draw_fill_line(
    terminalpp::canvas_view &cvs,
    terminalpp::point const &origin,
    terminalpp::coordinate_type const &width,
    terminalpp::element const &fill)
{
    for (terminalpp::coordinate_type column = origin.x;
         column < origin.x + width;
         ++column)
    {
        cvs[column][origin.y] = fill;
    }
}

// ==========================================================================
// DRAW_CONTENT_LINE
// ==========================================================================
static void draw_content_line(
    terminalpp::canvas_view &cvs,
    terminalpp::point const &origin,
    terminalpp::coordinate_type const &content_start,
    terminalpp::coordinate_type const &line_width,
    terminalpp::string const &content,
    terminalpp::element const &fill)
{
    for (auto column = origin.x; column < origin.x + line_width; ++column)
    {
        bool const column_has_content =
            column >= content_start
         && column <  content_start + content.size();
         
        cvs[column][origin.y] = 
            column_has_content
          ? content[column - content_start]
          : fill;
    }
}


// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::element fill)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->fill_ = fill;
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
    auto &cvs = ctx.get_canvas();

    auto const size = get_size();
    auto const content_size = get_preferred_size();

    auto const content_basis = terminalpp::point {
        (size.width - content_size.width) / 2,
        (size.height - content_size.height) / 2
    };
    
    for (terminalpp::coordinate_type row = region.origin.y;
         row < region.origin.y + region.size.height;
         ++row)
    {
        bool const row_has_content = 
            row >= content_basis.y
         && row < content_basis.y + pimpl_->content_.size();

        if (row_has_content)
        {
            draw_content_line(
                cvs,
                { region.origin.x, row },
                content_basis.x,
                region.size.width,
                pimpl_->content_[row - content_basis.y],
                pimpl_->fill_);
        }
        else
        {
            draw_fill_line(
                cvs, 
                { region.origin.x, row },
                region.size.width,
                pimpl_->fill_);
        }
    }
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

