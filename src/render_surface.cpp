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
    render_surface::row_proxy const &other)
{
    surface_.set_element(column_, row_, other);
    return *this;
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
// ROW_PROXY::CONVERSION OPERATOR
// ==========================================================================
render_surface::row_proxy::operator terminalpp::element &()
{
    return surface_.get_element(column_, row_);
}

// ==========================================================================
// ROW_PROXY::CONVERSION OPERATOR
// ==========================================================================
render_surface::row_proxy::operator terminalpp::element const &() const
{
    return surface_.get_element(column_, row_);
}

// ==========================================================================
// ROW_PROXY::OPERATOR==
// ==========================================================================
bool render_surface::row_proxy::operator==(
    terminalpp::element const& value) const
{
    return surface_.get_element(column_, row_) == value;
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
// CONST_COLUMN_PROXY::CONSTRUCTOR
// ==========================================================================
render_surface::const_column_proxy::const_column_proxy(
    render_surface const &surface, 
    terminalpp::coordinate_type column)
  : surface_(surface),
    column_(column)
{
}

// ==========================================================================
// CONST_COLUMN_PROXY::OPERATOR[]
// ==========================================================================
terminalpp::element const &render_surface::const_column_proxy::operator[](
    terminalpp::coordinate_type row) const
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
// OPERATOR[]
// ==========================================================================
render_surface::const_column_proxy render_surface::operator[](
    terminalpp::coordinate_type column) const
{
    return const_column_proxy(*this, column);
}

// ==========================================================================
// GET_ELEMENT
// ==========================================================================
terminalpp::element &render_surface::get_element(
    terminalpp::coordinate_type column, terminalpp::coordinate_type row)
{
    return canvas_[column + offset_.width][row + offset_.height];
}

// ==========================================================================
// GET_ELEMENT
// ==========================================================================
terminalpp::element const &render_surface::get_element(
    terminalpp::coordinate_type column, 
    terminalpp::coordinate_type row) const
{
    return canvas_[column + offset_.width][row + offset_.height];
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

// ==========================================================================
// OPERATOR<<(row_proxy)
// ==========================================================================
std::ostream &operator<<(
    std::ostream &out,
    render_surface::row_proxy const &row)
{
    return out << terminalpp::element(row);
}

}
