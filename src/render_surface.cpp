#include "munin/render_surface.hpp"

namespace munin {

default_render_surface_capabilities default_capabilities;

// ==========================================================================
// COLUMN_PROXY::CONSTRUCTOR
// ==========================================================================
render_surface::column_proxy::column_proxy(
    render_surface &surface, terminalpp::coordinate_type column)
  : surface_(surface), column_(column)
{
}

// ==========================================================================
// COLUMN_PROXY::OPERATOR[]
// ==========================================================================
terminalpp::element &render_surface::column_proxy::operator[](
    terminalpp::coordinate_type row)
{
  return surface_.get_element(column_, row);
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
render_surface::render_surface(terminalpp::canvas &cvs)
  : render_surface(cvs, default_capabilities)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
render_surface::render_surface(
    terminalpp::canvas &cvs, render_surface_capabilities const &capabilities)
  : canvas_(cvs), capabilities_(capabilities)
{
}

// ==========================================================================
// SUPPORTS_UNICODE
// ==========================================================================
bool render_surface::supports_unicode() const
{
  return capabilities_.supports_unicode();
}

// ==========================================================================
// OFFSET_BY
// ==========================================================================
void render_surface::offset_by(terminalpp::extent offset)
{
  offset_ += offset;
}

// ==========================================================================
// SIZE
// ==========================================================================
terminalpp::extent render_surface::size() const
{
  return canvas_.size() - offset_;
}

// ==========================================================================
// OPERATOR[]
// ==========================================================================
render_surface::column_proxy render_surface::operator[](
    terminalpp::coordinate_type column)
{
  return {*this, column};
}

// ==========================================================================
// GET_ELEMENT
// ==========================================================================
terminalpp::element &render_surface::get_element(
    terminalpp::coordinate_type column, terminalpp::coordinate_type row)
{
  return canvas_[column + offset_.width_][row + offset_.height_];
}

}  // namespace munin
