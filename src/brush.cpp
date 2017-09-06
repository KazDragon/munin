#include "munin/brush.hpp"
#include "munin/context.hpp"
#include <terminalpp/canvas_view.hpp>
#include <utility>

using namespace terminalpp::literals;

namespace munin {

// ==========================================================================
// BRUSH::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct brush::impl
{
    terminalpp::extent preferred_size_;
    std::vector<terminalpp::string> pattern_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush()
  : brush(" "_ts)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush(terminalpp::string pattern)
  : brush(std::vector<terminalpp::string>{pattern})
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
brush::brush(std::vector<terminalpp::string> pattern)
    : pimpl_(std::make_shared<impl>())
{
    pimpl_->preferred_size_ = terminalpp::extent(1, 1);
    pimpl_->pattern_ = std::move(pattern);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
brush::~brush()
{
}

// ==========================================================================
// SET_PREFERRED_SIZE
// ==========================================================================
void brush::set_preferred_size(terminalpp::extent preferred_size)
{
    pimpl_->preferred_size_ = preferred_size;
    on_preferred_size_changed();
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent brush::do_get_preferred_size() const
{
    return pimpl_->preferred_size_;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void brush::do_draw(context &ctx, rectangle const &region) const
{
    auto &cvs = ctx.get_canvas();

    for (terminalpp::coordinate_type row = region.origin.y;
         row < region.origin.y + region.size.height;
         ++row)
    {
        for (terminalpp::coordinate_type column = region.origin.x;
             column < region.origin.x + region.size.width;
             ++column)
        {
            auto const fill_row = row % pimpl_->pattern_.size();
            auto const fill_column = column % pimpl_->pattern_[fill_row].size();
            
            cvs[column][row] = pimpl_->pattern_[fill_row][fill_column];
        }
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json brush::do_to_json() const
{
    /*
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "brush" }
    ])"_json;

    return basic_component::do_to_json().patch(patch);
    */
    
    return basic_component::do_to_json();
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush()
{
    return std::make_shared<brush>();
}

}

