#include "munin/detail/adaptive_fill.hpp"

#include "munin/detail/border_glyphs.hpp"
#include "munin/render_surface.hpp"

namespace munin::detail {
namespace {

// ==========================================================================
// SELECT_TOP_LEFT_CORNER_GLYPH
// ==========================================================================
auto select_top_left_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode() ? detail::border::unicode::top_left_corner
                                      : detail::border::ansi::top_left_corner;
}

// ==========================================================================
// SELECT_TOP_RIGHT_CORNER_GLYPH
// ==========================================================================
auto select_top_right_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
             ? detail::border::unicode::top_right_corner
             : detail::border::ansi::top_right_corner;
}

// ==========================================================================
// SELECT_BOTTOM_LEFT_CORNER_GLYPH
// ==========================================================================
auto select_bottom_left_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
             ? detail::border::unicode::bottom_left_corner
             : detail::border::ansi::bottom_left_corner;
}

// ==========================================================================
// SELECT_BOTTOM_RIGHT_CORNER_GLYPH
// ==========================================================================
auto select_bottom_right_corner_glyph(render_surface const &surface)
{
    return surface.supports_unicode()
             ? detail::border::unicode::bottom_right_corner
             : detail::border::ansi::bottom_right_corner;
}

// ==========================================================================
// SELECT_HORIZONTAL_BEAM_GLYPH
// ==========================================================================
auto select_horizontal_beam_glyph(render_surface const &surface)
{
    return surface.supports_unicode() ? detail::border::unicode::horizontal_beam
                                      : detail::border::ansi::horizontal_beam;
}

// ==========================================================================
// SELECT_VERTICAL_BEAM_GLYPH
// ==========================================================================
auto select_vertical_beam_glyph(render_surface const &surface)
{
    return surface.supports_unicode() ? detail::border::unicode::vertical_beam
                                      : detail::border::ansi::vertical_beam;
}

}  // namespace

// ==========================================================================
// MAKE_TOP_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_left_corner_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{select_top_left_corner_glyph(surface), attr};
    });
}

// ==========================================================================
// MAKE_HORIZONTAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_horizontal_beam_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{select_horizontal_beam_glyph(surface), attr};
    });
}

// ==========================================================================
// MAKE_TOP_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_top_right_corner_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{
            select_top_right_corner_glyph(surface), attr};
    });
}

// ==========================================================================
// MAKE_VERTICAL_BEAM_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_vertical_beam_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{select_vertical_beam_glyph(surface), attr};
    });
}

// ==========================================================================
// MAKE_BOTTOM_LEFT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_left_corner_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{
            select_bottom_left_corner_glyph(surface), attr};
    });
}

// ==========================================================================
// MAKE_BOTTOM_RIGHT_CORNER_FILL
// ==========================================================================
std::shared_ptr<munin::filled_box> make_bottom_right_corner_fill(
    terminalpp::attribute const &attr)
{
    return munin::make_fill([&attr](render_surface &surface) {
        return terminalpp::element{
            select_bottom_right_corner_glyph(surface), attr};
    });
}

}  // namespace munin::detail
