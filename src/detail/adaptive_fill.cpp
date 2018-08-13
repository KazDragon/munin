#include "munin/detail/adaptive_fill.hpp"
#include "munin/detail/unicode_glyphs.hpp"
#include "munin/render_surface.hpp"

namespace munin { namespace detail {
namespace {

constexpr terminalpp::glyph const default_corner_glyph          = '+';
constexpr terminalpp::glyph const default_horizontal_beam_glyph = '-';
constexpr terminalpp::glyph const default_vertical_beam_glyph   = '|';

// ==========================================================================
// SELECT_TOP_LEFT_CORNER_GLYPH
// ==========================================================================
auto select_top_left_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_rounded_top_left_corner
         : default_corner_glyph;
}

// ==========================================================================
// SELECT_TOP_RIGHT_CORNER_GLYPH
// ==========================================================================
auto select_top_right_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_rounded_top_right_corner
         : default_corner_glyph;
}

// ==========================================================================
// SELECT_BOTTOM_LEFT_CORNER_GLYPH
// ==========================================================================
auto select_bottom_left_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_rounded_bottom_left_corner
         : default_corner_glyph;
}

// ==========================================================================
// SELECT_BOTTOM_RIGHT_CORNER_GLYPH
// ==========================================================================
auto select_bottom_right_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_rounded_bottom_right_corner
         : default_corner_glyph;
}

// ==========================================================================
// SELECT_HORIZONTAL_BEAM_GLYPH
// ==========================================================================
auto select_horizontal_beam_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_horizontal_beam
         : default_horizontal_beam_glyph;
}

// ==========================================================================
// SELECT_VERTICAL_BEAM_GLYPH
// ==========================================================================
auto select_vertical_beam_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
         ? detail::single_lined_vertical_beam
         : default_vertical_beam_glyph;
}

}

// ==========================================================================
// MAKE_TOP_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_left_corner_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_top_left_corner_glyph(surface), 
                *attr
            };
        });
}

// ==========================================================================
// MAKE_HORIZONTAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_horizontal_beam_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_horizontal_beam_glyph(surface), 
                *attr
            };
        });
}

// ==========================================================================
// MAKE_TOP_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_right_corner_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_top_right_corner_glyph(surface), 
                *attr
            };
        });
}

// ==========================================================================
// MAKE_VERTICAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_vertical_beam_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_vertical_beam_glyph(surface), 
                *attr
            };
        });
}

// ==========================================================================
// MAKE_BOTTOM_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_left_corner_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_bottom_left_corner_glyph(surface), 
                *attr
            };
        });
}

// ==========================================================================
// MAKE_BOTTOM_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_right_corner_fill(
    terminalpp::attribute *&attr)
{
    return munin::make_fill(
        [&attr](render_surface &surface)
        {
            return terminalpp::element{
                select_bottom_right_corner_glyph(surface), 
                *attr
            };
        });
}
    
}}
