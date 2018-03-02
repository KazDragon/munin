#include "munin/brush.hpp"
#include <terminalpp/canvas_view.hpp>
#include <utility>

using namespace terminalpp::literals;

namespace munin {

// ==========================================================================
// BRUSH::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct brush::impl
{
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
    pimpl_->pattern_ = std::move(pattern);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
brush::~brush()
{
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern()
{
    set_pattern(" ");
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern(terminalpp::string const &pattern)
{
    set_pattern(std::vector<terminalpp::string>{pattern});
}

// ==========================================================================
// SET_PATTERN
// ==========================================================================
void brush::set_pattern(std::vector<terminalpp::string> const &pattern)
{
    pimpl_->pattern_ = pattern;
    on_preferred_size_changed();
    on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent brush::do_get_preferred_size() const
{
    return pimpl_->pattern_.empty()
         ? terminalpp::extent(1, 1)
         : terminalpp::extent(
               std::max_element(
                   pimpl_->pattern_.begin(),
                   pimpl_->pattern_.end(),
                   [](auto const &lhs, auto const &rhs)
                   {
                       return lhs.size() < rhs.size();
                   })->size(),
               pimpl_->pattern_.size());
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void brush::do_draw(
    terminalpp::canvas_view &cvs, rectangle const &region) const
{
    for (terminalpp::coordinate_type row = region.origin.y;
         row < region.origin.y + region.size.height;
         ++row)
    {
        auto const fill_row = row % pimpl_->pattern_.size();

        for (terminalpp::coordinate_type column = region.origin.x;
             column < region.origin.x + region.size.width;
             ++column)
        {
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
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "brush" }
    ])"_json;

    auto json = basic_component::do_to_json().patch(patch);

    json["pattern"]["size"] = pimpl_->pattern_.size();

    for (size_t index = 0; index < pimpl_->pattern_.size(); ++index)
    {
        json["pattern"]["content"][index] =
            terminalpp::to_string(pimpl_->pattern_[index]);
    }

    return json;
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush()
{
    return std::make_shared<brush>();
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush(terminalpp::string pattern)
{
    return std::make_shared<brush>(std::move(pattern));
}

// ==========================================================================
// MAKE_BRUSH
// ==========================================================================
std::shared_ptr<brush> make_brush(std::vector<terminalpp::string> pattern)
{
    return std::make_shared<brush>(std::move(pattern));
}

}
