#pragma once

#include <terminalpp/glyph.hpp>

namespace munin { namespace detail { inline namespace unicode {

// Some uncommon glyphs that are very useful.

static constexpr terminalpp::glyph const single_lined_top_left_corner("\U0000250C");
static constexpr terminalpp::glyph const single_lined_top_right_corner("\U00002510");
static constexpr terminalpp::glyph const single_lined_bottom_left_corner("\U00002514");
static constexpr terminalpp::glyph const single_lined_bottom_right_corner("\U00002518");

static constexpr terminalpp::glyph const single_lined_rounded_top_left_corner("\U0000256D");
static constexpr terminalpp::glyph const single_lined_rounded_top_right_corner("\U0000256E");
static constexpr terminalpp::glyph const single_lined_rounded_bottom_left_corner("\U00002570");
static constexpr terminalpp::glyph const single_lined_rounded_bottom_right_corner("\U0000256F");

static constexpr terminalpp::glyph const single_lined_horizontal_beam("\U00002500");
static constexpr terminalpp::glyph const single_lined_vertical_beam("\U00002502");
static constexpr terminalpp::glyph const single_lined_top_tee("\U0000252C");
static constexpr terminalpp::glyph const single_lined_left_tee("\U0000251C");
static constexpr terminalpp::glyph const single_lined_bottom_tee("\U00002534");
static constexpr terminalpp::glyph const single_lined_right_tee("\U00002524");
static constexpr terminalpp::glyph const single_lined_cross("\U0000253C");

// Double-lined box drawing
static constexpr terminalpp::glyph const double_lined_top_left_corner("\U00002554");
static constexpr terminalpp::glyph const double_lined_top_right_corner("\U00002557");
static constexpr terminalpp::glyph const double_lined_bottom_left_corner("\U0000255A");
static constexpr terminalpp::glyph const double_lined_bottom_right_corner("\U0000255D");
static constexpr terminalpp::glyph const double_lined_horizontal_beam("\U00002550");
static constexpr terminalpp::glyph const double_lined_vertical_beam("\U00002551");
static constexpr terminalpp::glyph const double_lined_top_tee("\U00002566");
static constexpr terminalpp::glyph const double_lined_left_tee("\U00002560");
static constexpr terminalpp::glyph const double_lined_bottom_tee("\U00002569");
static constexpr terminalpp::glyph const double_lined_right_tee("\U00002563");
static constexpr terminalpp::glyph const double_lined_cross("\U0000256C");

// Mix-lined box drawing.  v and h describe whether the single-line component
// is horizontal or vertical
static constexpr terminalpp::glyph const mix_lined_vright_tee("\U00002561");
static constexpr terminalpp::glyph const mix_lined_hright_tee("\U00002562");
static constexpr terminalpp::glyph const mix_lined_vtop_right_corner("\U00002555");
static constexpr terminalpp::glyph const mix_lined_htop_right_corner("\U00002556");
static constexpr terminalpp::glyph const mix_lined_vbottom_right_corner("\U0000255B");
static constexpr terminalpp::glyph const mix_lined_hbottom_right_corner("\U0000255C");
static constexpr terminalpp::glyph const mix_lined_vleft_tee("\U0000255E");
static constexpr terminalpp::glyph const mix_lined_hleft_tee("\U0000255F");
static constexpr terminalpp::glyph const mix_lined_vbottom_tee("\U00002567");
static constexpr terminalpp::glyph const mix_lined_hbottom_tee("\U00002568");
static constexpr terminalpp::glyph const mix_lined_vtop_tee("\U00002564");
static constexpr terminalpp::glyph const mix_lined_htop_tee("\U00002565");
static constexpr terminalpp::glyph const mix_lined_vbottom_left_corner("\U00002559");
static constexpr terminalpp::glyph const mix_lined_hbottom_left_corner("\U0000255A");
static constexpr terminalpp::glyph const mix_lined_vtop_left_corner("\U00002552");
static constexpr terminalpp::glyph const mix_lined_htop_left_corner("\U00002553");
static constexpr terminalpp::glyph const mix_lined_hcross("\U0000256B");
static constexpr terminalpp::glyph const mix_lined_vcross("\U0000256A");

// Other drawing characters.
static constexpr terminalpp::glyph const left_guillemet("\U000000AB");
static constexpr terminalpp::glyph const right_guillemet("\U000000BB");
static constexpr terminalpp::glyph const light_cross("\U00002573");
static constexpr terminalpp::glyph const down_pointing_triangle("\U000025BE");

}}}
