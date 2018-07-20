#include "munin/render_surface.hpp"

namespace munin {
namespace {

// ==========================================================================
// DEFAULT RENDER SURFACE CAPABILITIES
// ==========================================================================
class default_render_surface_capabilities
  : public render_surface_capabilities
{
public :
    // ======================================================================
    // SUPPORTS_UNICODE
    // ======================================================================
    bool supports_unicode() const override
    {
        return true;
    }
};

static default_render_surface_capabilities default_capabilities;

}

// ==========================================================================
// ROW_PROXY::CONSTRUCTOR
// ==========================================================================
render_surface::row_proxy::row_proxy(
    render_surface& surface, 
    terminalpp::coordinate_type column, 
    terminalpp::coordinate_type row)
  : surface_(surface),
    column_(column),
    row_(row)
{
}

// ==========================================================================
// ROW_PROXY::OPERATOR=
// ==========================================================================
render_surface::row_proxy &render_surface::row_proxy::operator=(
    terminalpp::element const &value)
{
    surface_.set_element(column_, row_, value);
    return *this;
}

// ==========================================================================
// COLUMN_PROXY::CONSTRUCTOR
// ==========================================================================
render_surface::column_proxy::column_proxy(
    render_surface &surface, 
    terminalpp::coordinate_type column)
  : surface_(surface),
    column_(column)
{
}

// ==========================================================================
// COLUMN_PROXY::OPERATOR[]
// ==========================================================================
render_surface::row_proxy render_surface::column_proxy::operator[](
    terminalpp::coordinate_type row)
{
    return render_surface::row_proxy(surface_, column_, row);
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
    terminalpp::canvas &cvs,
    render_surface_capabilities const &capabilities)
  : canvas_(cvs),
    capabilities_(capabilities)
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
void render_surface::offset_by(
    terminalpp::extent offset)
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
    return column_proxy(*this, column);
}

// ==========================================================================
// SET_ELEMENT
// ==========================================================================
void render_surface::set_element(
    terminalpp::coordinate_type column, 
    terminalpp::coordinate_type row, 
    terminalpp::element const &value)
{
    canvas_[column + offset_.width][row + offset_.height] = value;
}

}
