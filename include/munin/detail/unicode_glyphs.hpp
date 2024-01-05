#pragma once

#include <terminalpp/glyph.hpp>

namespace munin::detail {
inline namespace unicode {

// Some uncommon glyphs that are very useful.
// clang-format off
static constexpr terminalpp::glyph const single_lined_top_left_corner(u8"\U0000250C");
static constexpr terminalpp::glyph const single_lined_top_right_corner(u8"\U00002510");
static constexpr terminalpp::glyph const single_lined_bottom_left_corner(u8"\U00002514");
static constexpr terminalpp::glyph const single_lined_bottom_right_corner(u8"\U00002518");

static constexpr terminalpp::glyph const single_lined_rounded_top_left_corner(u8"\U0000256D");
static constexpr terminalpp::glyph const single_lined_rounded_top_right_corner(u8"\U0000256E");
static constexpr terminalpp::glyph const single_lined_rounded_bottom_left_corner(u8"\U00002570");
static constexpr terminalpp::glyph const single_lined_rounded_bottom_right_corner(u8"\U0000256F");

static constexpr terminalpp::glyph const single_lined_horizontal_beam(u8"\U00002500");
static constexpr terminalpp::glyph const single_lined_vertical_beam(u8"\U00002502");
static constexpr terminalpp::glyph const single_lined_top_tee(u8"\U0000252C");
static constexpr terminalpp::glyph const single_lined_left_tee(u8"\U0000251C");
static constexpr terminalpp::glyph const single_lined_bottom_tee(u8"\U00002534");
static constexpr terminalpp::glyph const single_lined_right_tee(u8"\U00002524");
static constexpr terminalpp::glyph const single_lined_cross(u8"\U0000253C");

// Double-lined box drawing
static constexpr terminalpp::glyph const double_lined_top_left_corner(u8"\U00002554");
static constexpr terminalpp::glyph const double_lined_top_right_corner(u8"\U00002557");
static constexpr terminalpp::glyph const double_lined_bottom_left_corner(u8"\U0000255A");
static constexpr terminalpp::glyph const double_lined_bottom_right_corner(u8"\U0000255D");
static constexpr terminalpp::glyph const double_lined_horizontal_beam(u8"\U00002550");
static constexpr terminalpp::glyph const double_lined_vertical_beam(u8"\U00002551");
static constexpr terminalpp::glyph const double_lined_top_tee(u8"\U00002566");
static constexpr terminalpp::glyph const double_lined_left_tee(u8"\U00002560");
static constexpr terminalpp::glyph const double_lined_bottom_tee(u8"\U00002569");
static constexpr terminalpp::glyph const double_lined_right_tee(u8"\U00002563");
static constexpr terminalpp::glyph const double_lined_cross(u8"\U0000256C");

// Mix-lined box drawing.  v and h describe whether the single-line component
// is horizontal or vertical
static constexpr terminalpp::glyph const mix_lined_vright_tee(u8"\U00002561");
static constexpr terminalpp::glyph const mix_lined_hright_tee(u8"\U00002562");
static constexpr terminalpp::glyph const mix_lined_vtop_right_corner(u8"\U00002555");
static constexpr terminalpp::glyph const mix_lined_htop_right_corner(u8"\U00002556");
static constexpr terminalpp::glyph const mix_lined_vbottom_right_corner(u8"\U0000255B");
static constexpr terminalpp::glyph const mix_lined_hbottom_right_corner(u8"\U0000255C");
static constexpr terminalpp::glyph const mix_lined_vleft_tee(u8"\U0000255E");
static constexpr terminalpp::glyph const mix_lined_hleft_tee(u8"\U0000255F");
static constexpr terminalpp::glyph const mix_lined_vbottom_tee(u8"\U00002567");
static constexpr terminalpp::glyph const mix_lined_hbottom_tee(u8"\U00002568");
static constexpr terminalpp::glyph const mix_lined_vtop_tee(u8"\U00002564");
static constexpr terminalpp::glyph const mix_lined_htop_tee(u8"\U00002565");
static constexpr terminalpp::glyph const mix_lined_vbottom_left_corner(u8"\U00002559");
static constexpr terminalpp::glyph const mix_lined_hbottom_left_corner(u8"\U0000255A");
static constexpr terminalpp::glyph const mix_lined_vtop_left_corner(u8"\U00002552");
static constexpr terminalpp::glyph const mix_lined_htop_left_corner(u8"\U00002553");
static constexpr terminalpp::glyph const mix_lined_hcross(u8"\U0000256B");
static constexpr terminalpp::glyph const mix_lined_vcross(u8"\U0000256A");

// Other drawing characters.
static constexpr terminalpp::glyph const left_guillemet(u8"\U000000AB");
static constexpr terminalpp::glyph const right_guillemet(u8"\U000000BB");
static constexpr terminalpp::glyph const light_cross(u8"\U00002573");
static constexpr terminalpp::glyph const down_pointing_triangle(u8"\U000025BE");
// clang-format on
}  // namespace unicode
}  // namespace munin::detail
