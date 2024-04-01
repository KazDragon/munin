#include "munin/detail/unicode_glyphs.hpp"

#include <terminalpp/glyph.hpp>

namespace munin::detail::border {

namespace ansi {

constexpr terminalpp::glyph top_left_corner = '+';
constexpr terminalpp::glyph top_right_corner = '+';
constexpr terminalpp::glyph bottom_left_corner = '+';
constexpr terminalpp::glyph bottom_right_corner = '+';
constexpr terminalpp::glyph horizontal_beam = '-';
constexpr terminalpp::glyph vertical_beam = '|';
constexpr terminalpp::glyph cross = '+';

}  // namespace ansi

namespace unicode {

constexpr terminalpp::glyph top_left_corner =
    single_lined_rounded_top_left_corner;
constexpr terminalpp::glyph top_right_corner =
    single_lined_rounded_top_right_corner;
constexpr terminalpp::glyph bottom_left_corner =
    single_lined_rounded_bottom_left_corner;
constexpr terminalpp::glyph bottom_right_corner =
    single_lined_rounded_bottom_right_corner;
constexpr terminalpp::glyph horizontal_beam = single_lined_horizontal_beam;
constexpr terminalpp::glyph vertical_beam = single_lined_vertical_beam;
constexpr terminalpp::glyph cross = single_lined_cross;

}  // namespace unicode

}  // namespace munin::detail::border