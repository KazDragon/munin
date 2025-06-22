#pragma once

#include <terminalpp/glyph.hpp>

namespace munin::detail {
inline namespace unicode {

// Some uncommon glyphs that are very useful.
// clang-format off
inline constexpr terminalpp::glyph const single_lined_top_left_corner(u8"\U0000250C");
inline constexpr terminalpp::glyph const single_lined_top_right_corner(u8"\U00002510");
inline constexpr terminalpp::glyph const single_lined_bottom_left_corner(u8"\U00002514");
inline constexpr terminalpp::glyph const single_lined_bottom_right_corner(u8"\U00002518");

inline constexpr terminalpp::glyph const single_lined_rounded_top_left_corner(u8"\U0000256D");
inline constexpr terminalpp::glyph const single_lined_rounded_top_right_corner(u8"\U0000256E");
inline constexpr terminalpp::glyph const single_lined_rounded_bottom_left_corner(u8"\U00002570");
inline constexpr terminalpp::glyph const single_lined_rounded_bottom_right_corner(u8"\U0000256F");

inline constexpr terminalpp::glyph const single_lined_horizontal_beam(u8"\U00002500");
inline constexpr terminalpp::glyph const single_lined_vertical_beam(u8"\U00002502");
inline constexpr terminalpp::glyph const single_lined_top_tee(u8"\U0000252C");
inline constexpr terminalpp::glyph const single_lined_left_tee(u8"\U0000251C");
inline constexpr terminalpp::glyph const single_lined_bottom_tee(u8"\U00002534");
inline constexpr terminalpp::glyph const single_lined_right_tee(u8"\U00002524");
inline constexpr terminalpp::glyph const single_lined_cross(u8"\U0000253C");

// Double-lined box drawing
inline constexpr terminalpp::glyph const double_lined_top_left_corner(u8"\U00002554");
inline constexpr terminalpp::glyph const double_lined_top_right_corner(u8"\U00002557");
inline constexpr terminalpp::glyph const double_lined_bottom_left_corner(u8"\U0000255A");
inline constexpr terminalpp::glyph const double_lined_bottom_right_corner(u8"\U0000255D");
inline constexpr terminalpp::glyph const double_lined_horizontal_beam(u8"\U00002550");
inline constexpr terminalpp::glyph const double_lined_vertical_beam(u8"\U00002551");
inline constexpr terminalpp::glyph const double_lined_top_tee(u8"\U00002566");
inline constexpr terminalpp::glyph const double_lined_left_tee(u8"\U00002560");
inline constexpr terminalpp::glyph const double_lined_bottom_tee(u8"\U00002569");
inline constexpr terminalpp::glyph const double_lined_right_tee(u8"\U00002563");
inline constexpr terminalpp::glyph const double_lined_cross(u8"\U0000256C");

// Mix-lined box drawing.  v and h describe whether the single-line component
// is horizontal or vertical
inline constexpr terminalpp::glyph const mix_lined_vright_tee(u8"\U00002561");
inline constexpr terminalpp::glyph const mix_lined_hright_tee(u8"\U00002562");
inline constexpr terminalpp::glyph const mix_lined_vtop_right_corner(u8"\U00002555");
inline constexpr terminalpp::glyph const mix_lined_htop_right_corner(u8"\U00002556");
inline constexpr terminalpp::glyph const mix_lined_vbottom_right_corner(u8"\U0000255B");
inline constexpr terminalpp::glyph const mix_lined_hbottom_right_corner(u8"\U0000255C");
inline constexpr terminalpp::glyph const mix_lined_vleft_tee(u8"\U0000255E");
inline constexpr terminalpp::glyph const mix_lined_hleft_tee(u8"\U0000255F");
inline constexpr terminalpp::glyph const mix_lined_vbottom_tee(u8"\U00002567");
inline constexpr terminalpp::glyph const mix_lined_hbottom_tee(u8"\U00002568");
inline constexpr terminalpp::glyph const mix_lined_vtop_tee(u8"\U00002564");
inline constexpr terminalpp::glyph const mix_lined_htop_tee(u8"\U00002565");
inline constexpr terminalpp::glyph const mix_lined_vbottom_left_corner(u8"\U00002559");
inline constexpr terminalpp::glyph const mix_lined_hbottom_left_corner(u8"\U0000255A");
inline constexpr terminalpp::glyph const mix_lined_vtop_left_corner(u8"\U00002552");
inline constexpr terminalpp::glyph const mix_lined_htop_left_corner(u8"\U00002553");
inline constexpr terminalpp::glyph const mix_lined_hcross(u8"\U0000256B");
inline constexpr terminalpp::glyph const mix_lined_vcross(u8"\U0000256A");

// Other drawing characters.
inline constexpr terminalpp::glyph const left_guillemet(u8"\U000000AB");
inline constexpr terminalpp::glyph const right_guillemet(u8"\U000000BB");
inline constexpr terminalpp::glyph const light_cross(u8"\U00002573");
inline constexpr terminalpp::glyph const down_pointing_triangle(u8"\U000025BE");
// clang-format on
}  // namespace unicode
}  // namespace munin::detail
