#include "munin/filled_box.hpp"
#include <terminalpp/canvas_view.hpp>

namespace munin {

// ==========================================================================
// FILLED_BOX::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct filled_box::impl
{
    terminalpp::element element_;
    terminalpp::extent  preferred_size_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
filled_box::filled_box(terminalpp::element const &element)
    : pimpl_(std::make_shared<impl>())
{
    pimpl_->element_        = element;
    pimpl_->preferred_size_ = terminalpp::extent(1, 1);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
filled_box::~filled_box()
{
}

// ==========================================================================
// SET_PREFERRED_SIZE
// ==========================================================================
void filled_box::set_preferred_size(terminalpp::extent preferred_size)
{
    pimpl_->preferred_size_ = preferred_size;
    on_preferred_size_changed();
}

// ==========================================================================
// SET_FILL
// ==========================================================================
void filled_box::set_fill(terminalpp::element const &element)
{
    pimpl_->element_ = element;
    on_redraw({rectangle({}, get_size())});
}

// ==========================================================================
// GET_FILL
// ==========================================================================
terminalpp::element filled_box::get_fill() const
{
    return pimpl_->element_;
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent filled_box::do_get_preferred_size() const
{
    return pimpl_->preferred_size_;
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void filled_box::do_draw(
    terminalpp::canvas_view &cvs, rectangle const &region) const
{
    for (terminalpp::coordinate_type row = region.origin.y;
         row < region.origin.y + region.size.height;
         ++row)
    {
        for (terminalpp::coordinate_type column = region.origin.x;
             column < region.origin.x + region.size.width;
             ++column)
        {
            cvs[column][row] = pimpl_->element_;
        }
    }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json filled_box::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "filled_box" }
    ])"_json;

    return basic_component::do_to_json().patch(patch);
}

// ==========================================================================
// MAKE_FILLED_BOX
// ==========================================================================
std::shared_ptr<filled_box> make_fill(terminalpp::element const &fill)
{
    return std::make_shared<filled_box>(fill);
}

}

