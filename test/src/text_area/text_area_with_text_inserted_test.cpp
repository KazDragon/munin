#include "text_area_test.hpp"
#include <terminalpp/mouse.hpp>
#include <terminalpp/virtual_key.hpp>
#include <munin/render_surface.hpp>

using testing::ValuesIn;

class a_text_area_with_text_inserted : public a_text_area
{
public:
    a_text_area_with_text_inserted()
    {
        text_area_.insert_text("ab");
    }
};

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_at_the_front)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("c", 0);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'c'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'a'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[2][0]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_in_the_middle)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("c", 1);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'c'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[2][0]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, can_have_text_inserted_that_pushes_existing_text_into_the_next_line)
{
    text_area_.set_size({4, 2});

    bool redraw_requested = false;
    std::vector<terminalpp::rectangle> redraw_regions;
    
    text_area_.on_redraw.connect(
        [&](auto const &regions)
        {
            redraw_requested = true;
            redraw_regions = regions;
        });

    text_area_.insert_text("defg", 1);
    
    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};
    
    for (auto const &region : redraw_regions)
    {
        text_area_.draw(surface, region);
    }

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'d'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{'f'}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{'g'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);
    
    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, lays_out_its_text_when_the_size_changes)
{
    text_area_.set_size({4, 2});

    fill_canvas({5, 3});
    munin::render_surface surface{canvas_};

    text_area_.draw(surface, {{0, 0}, {4, 2}});

    ASSERT_EQ(terminalpp::element{'a'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'b'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);

    auto const expected_cursor_position = terminalpp::point{2, 0};
    ASSERT_EQ(expected_cursor_position, text_area_.get_cursor_position());

    verify_oob_is_untouched();
}

TEST_F(a_text_area_with_text_inserted, announces_a_change_in_preferred_size_if_its_size_is_changed_such_that_there_are_now_more_lines)
{
    // for example, if I change the size of "ab" to 1x1 such that the b would
    // now bleed into the next line, the text area should request the new
    // preferred height.
    text_area_.set_size({2, 1});

    auto preferred_size = terminalpp::extent{};
    text_area_.on_preferred_size_changed.connect(
        [&]
        {
            preferred_size = text_area_.get_preferred_size();
        });

    text_area_.set_size({1, 1});

    auto const expected_preferred_size = terminalpp::extent{1, 3};
    ASSERT_EQ(expected_preferred_size, preferred_size);
}

namespace {

using move_caret_test_data = std::tuple<
    munin::text_area::text_index, // caret position to set
    terminalpp::point,            // expected cursor position
    char                          // expected character at the caret
>;

class a_text_area_with_many_lines_of_text_inserted_base
  : public a_text_area_base
{
public:
    a_text_area_with_many_lines_of_text_inserted_base()
    {
        text_area_.insert_text(
          //                            +-- width
          // 0         1         2      V  3
          // 0123456789012345678901234567890123456789 X/Y
            "Lorem ipsum dolor sit amet,\n"          // 0 | 0   .. 27  (27)
            "consectetur adipiscing\n"               // 1 | 28  .. 50  (22)
            "elit. Proin sed nisl mattis,"           // 2 | 51  .. 78  (27)
          "\nluctus velit ullamcorper, mo"           // 3 | 79  .. 107 (27 + 1)
            "lestie\n"                               // 4 | 108 .. 114 (6)
            "mauris.\n"                              // 5 | 115 .. 122 (7)
            "");                                     // 6 | 123
            
        // Set the size of the text area so that the width is precisely on
        // a newline boundary.
        text_area_.set_size({28, 10}); // See "elit.  Proin sed nisl" line.
    }
};

class setting_the_caret_programatically
  : public a_text_area_with_many_lines_of_text_inserted_base,
    public testing::TestWithParam<move_caret_test_data>
{
};

}

TEST_P(setting_the_caret_programatically, moves_the_caret_and_cursor_as_specified)
{
    using std::get;
    
    auto const &params = GetParam();
    auto const &caret_position = get<0>(params);
    auto const &expected_cursor_position = get<1>(params);
    auto const &expected_character = get<2>(params);

    auto cursor_position = text_area_.get_cursor_position();
    text_area_.on_cursor_position_changed.connect(
        [this, &cursor_position]
        {
            cursor_position = text_area_.get_cursor_position();
        });

    text_area_.set_caret_position(caret_position);

    EXPECT_EQ(caret_position, text_area_.get_caret_position());
    EXPECT_EQ(expected_cursor_position, cursor_position);

    auto const text = text_area_.get_text();
    auto const character = 
        caret_position == text.size()
      ? '!'
      : text[caret_position].glyph_.character_;

    EXPECT_EQ(expected_character, character);
}

static move_caret_test_data const move_caret_test_entries[] =
{
    // Move to the beginning of the text
    move_caret_test_data{ 0,   {0,  0}, 'L'  },
    move_caret_test_data{ 1,   {1,  0}, 'o'  },
    move_caret_test_data{ 2,   {2,  0}, 'r'  },
    move_caret_test_data{ 3,   {3,  0}, 'e'  },
    move_caret_test_data{ 4,   {4,  0}, 'm'  },

    // Move to the end of the first line and the beginning of the next 
    // wrapped as determined by the newline.
    move_caret_test_data{ 26,  {26, 0}, ','  },
    move_caret_test_data{ 27,  {27, 0}, '\n' },
    move_caret_test_data{ 28,  {0,  1}, 'c'  },
    move_caret_test_data{ 29,  {1,  1}, 'o'  },

    move_caret_test_data{ 49,  {21, 1}, 'g'  },
    move_caret_test_data{ 50,  {22, 1}, '\n' },
    move_caret_test_data{ 51,  {0,  2}, 'e'  },
    move_caret_test_data{ 52,  {1,  2}, 'l'  },

    // Moving on the end of the third line, the caret is placed
    // precisely on the last character of the line.
    move_caret_test_data{ 78,  {27, 2}, ','  },
    move_caret_test_data{ 79,  {0,  3}, '\n' },
    move_caret_test_data{ 80,  {0,  3}, 'l'  },
    move_caret_test_data{ 81,  {1,  3}, 'u'  },

    // The text wraps at the end of the fourth line, which should
    // happen smoothly.
    move_caret_test_data{ 106, {26, 3}, 'm'  },
    move_caret_test_data{ 107, {27, 3}, 'o'  },
    move_caret_test_data{ 108, {0,  4}, 'l'  },
    move_caret_test_data{ 109, {1,  4}, 'e'  },

    // The end of the whole text should allow the caret in its own
    // line after the final newline.
    move_caret_test_data{ 121, {6,  5}, '.'  },
    move_caret_test_data{ 122, {7,  5}, '\n' },
    move_caret_test_data{ 123, {0,  6}, '!'  }, // Special empty character
};

INSTANTIATE_TEST_SUITE_P(
    allows_movement_of_the_caret,
    setting_the_caret_programatically,
    ValuesIn(move_caret_test_entries)
);

namespace {

using move_cursor_test_data = std::tuple<
    terminalpp::point,            // cursor set position
    terminalpp::point,            // expected cursor position
    munin::text_area::text_index, // expected caret position
    char                          // expected character under caret
>;

class setting_the_cursor_programatically
  : public a_text_area_with_many_lines_of_text_inserted_base,
    public testing::TestWithParam<move_cursor_test_data>
{
};

}

TEST_P(setting_the_cursor_programatically, moves_the_caret_and_cursor_as_specified)
{
    using std::get;

    auto const &params = GetParam();
    auto const &cursor_position = get<0>(params);
    auto const &expected_cursor_position = get<1>(params);
    auto const &expected_caret_position = get<2>(params);
    auto const &expected_character = get<3>(params);

    text_area_.set_cursor_position(cursor_position);

    EXPECT_EQ(expected_cursor_position, text_area_.get_cursor_position());
    EXPECT_EQ(expected_caret_position, text_area_.get_caret_position());

    auto const text = text_area_.get_text();
    auto const character = 
        expected_caret_position == text.size()
      ? '!'
      : text[expected_caret_position].glyph_.character_;

    EXPECT_EQ(expected_character, character);
}

static move_cursor_test_data const move_cursor_test_entries[] =
{
    // Move the cursor to the home point.
    move_cursor_test_data{ {0,  0}, {0,  0}, 0,   'L'  },

    // Move the cursor to arbitrary positions on the first row
    move_cursor_test_data{ {2,  0}, {2,  0}, 2,   'r'  },
    move_cursor_test_data{ {4,  0}, {4,  0}, 4,   'm'  },
    move_cursor_test_data{ {12, 0}, {12, 0}, 12,  'd'  },
    move_cursor_test_data{ {20, 0}, {20, 0}, 20,  't'  },
    move_cursor_test_data{ {27, 0}, {27, 0}, 27,  '\n' },

    // Overflow the cursor on the line should clip to the end of the line.
    move_cursor_test_data{ {28, 0}, {27, 0}, 27,  '\n' },

    // Move the cursor to arbitrary positions on the second row
    move_cursor_test_data{ {0,  1}, {0,  1}, 28,  'c'  },
    move_cursor_test_data{ {11, 1}, {11, 1}, 39,  ' '  },
    move_cursor_test_data{ {13, 1}, {13, 1}, 41,  'd'  },
    move_cursor_test_data{ {22, 1}, {22, 1}, 50,  '\n' },

    // Overflow the cursor on the line should clip to the end of the line.
    move_cursor_test_data{ {23, 1}, {22, 1}, 50,  '\n' },

    // Move the cursor to arbitrary positions on the third row
    move_cursor_test_data{ {0,  2}, {0,  2}, 51,  'e'  },
    move_cursor_test_data{ {11, 2}, {11, 2}, 62,  ' '  },
    move_cursor_test_data{ {13, 2}, {13, 2}, 64,  'e'  },
    move_cursor_test_data{ {27, 2}, {27, 2}, 78,  ','  },

    // Overflow the cursor on the line should clip to the end of the line.
    move_cursor_test_data{ {28, 2}, {27, 2}, 78,  ','  },

    // Move the cursor to arbitrary positions on the fourth row, which wrapped
    // from the third line, but started with a \n, which has been absorbed.
    // We assume that people clicking in a location want to edit that line and
    // not the line before, so the character underneath should not be the
    // newline.
    move_cursor_test_data{ {0,  3}, {0,  3}, 80,  'l'  },
    move_cursor_test_data{ {11, 3}, {11, 3}, 91,  't'  },
    move_cursor_test_data{ {13, 3}, {13, 3}, 93,  'u'  },
    move_cursor_test_data{ {27, 3}, {27, 3}, 107, 'o'  },

    // Overflow the cursor on the line should clip to the end of the line.
    move_cursor_test_data{ {28, 3}, {27, 3}, 107, 'o'  },

    // Move the character to the seventh empty row
    move_cursor_test_data{ {0,  6}, {0,  6}, 123, '!'  },

    // Overflow the cursor on the line should clip to the end of the line.
    move_cursor_test_data{ {1,  6}, {0,  6}, 123, '!'  },

    // Overflow the cursor downward should clip to the last entry
    move_cursor_test_data{ {0,  7}, {0,  6}, 123, '!'  },
    move_cursor_test_data{ {1,  7}, {0,  6}, 123, '!'  },
};

INSTANTIATE_TEST_SUITE_P(
    allows_movement_of_the_cursor,
    setting_the_cursor_programatically,
    ValuesIn(move_cursor_test_entries)
);

namespace {

class a_text_area_with_many_lines_of_text_inserted
  : public a_text_area_with_many_lines_of_text_inserted_base,
    public testing::Test
{
};

}

TEST_F(a_text_area_with_many_lines_of_text_inserted, moves_the_cursor_to_where_the_mouse_clicks)
{
    terminalpp::mouse::event event {
        terminalpp::mouse::event_type::left_button_down,
        {13, 1}
    };

    text_area_.event(event);

    auto const expected_cursor_position = terminalpp::point{13, 1};
    auto const expected_caret_position = munin::text_area::text_index{41};

    ASSERT_EQ(expected_cursor_position, text_area_.get_cursor_position());
    ASSERT_EQ(expected_caret_position, text_area_.get_caret_position());
}

namespace {

using movement_key_test_data = std::tuple<
    terminalpp::point,       // initial cursor position
    terminalpp::virtual_key, // keypress
    terminalpp::point        // eventual cursor location
>;

class pushing_a_movement_key
  : public a_text_area_with_many_lines_of_text_inserted_base,
    public testing::TestWithParam<movement_key_test_data>
{
};

}

TEST_P(pushing_a_movement_key, moves_the_cursor_as_described)
{
    using std::get;

    auto const &param = GetParam();
    auto const &initial_cursor_position = get<0>(param);
    auto const &keypress = get<1>(param);
    auto const &expected_cursor_position = get<2>(param);

    text_area_.set_cursor_position(initial_cursor_position);
    
    auto reported_cursor_position = text_area_.get_cursor_position();
    text_area_.on_cursor_position_changed.connect(
        [this, &reported_cursor_position]
        {
            reported_cursor_position = text_area_.get_cursor_position();
        });

    text_area_.event(keypress);

    ASSERT_EQ(expected_cursor_position, reported_cursor_position);
}

namespace {

template <int N>
static auto const keypress_cursor_left_n = terminalpp::virtual_key {
    terminalpp::vk::cursor_left,
    terminalpp::vk_modifier::none,
    N
};
static auto const keypress_cursor_left   = keypress_cursor_left_n<1>;
static auto const keypress_cursor_left_2 = keypress_cursor_left_n<2>;

template <int N>
static auto const keypress_cursor_right_n = terminalpp::virtual_key {
    terminalpp::vk::cursor_right,
    terminalpp::vk_modifier::none,
    N
};
static auto const keypress_cursor_right   = keypress_cursor_right_n<1>;
static auto const keypress_cursor_right_2 = keypress_cursor_right_n<2>;

template <int N>
static auto const keypress_cursor_up_n = terminalpp::virtual_key {
    terminalpp::vk::cursor_up,
    terminalpp::vk_modifier::none,
    N
};
static auto const keypress_cursor_up   = keypress_cursor_up_n<1>;
static auto const keypress_cursor_up_2 = keypress_cursor_up_n<2>;

template <int N>
static auto const keypress_cursor_down_n = terminalpp::virtual_key {
    terminalpp::vk::cursor_down,
    terminalpp::vk_modifier::none,
    N
};
static auto const keypress_cursor_down   = keypress_cursor_down_n<1>;
static auto const keypress_cursor_down_2 = keypress_cursor_down_n<2>;

static auto const keypress_home = terminalpp::virtual_key {
    terminalpp::vk::home,
    terminalpp::vk_modifier::none,
    1
};

static movement_key_test_data const move_key_test_entries[] =
{
    // Move the cursor left from various points
    movement_key_test_data{ {0,  0}, keypress_cursor_left,    {0,  0} },
    movement_key_test_data{ {1,  0}, keypress_cursor_left,    {0,  0} },
    movement_key_test_data{ {27, 0}, keypress_cursor_left,    {26, 0} },
    movement_key_test_data{ {0,  1}, keypress_cursor_left,    {27, 0} },

    // Move the cursor right from various points
    movement_key_test_data{ {0,  0}, keypress_cursor_right,   {1, 0} },

    movement_key_test_data{ {0,  1}, keypress_cursor_right,   {1, 1} },
    movement_key_test_data{ {5,  1}, keypress_cursor_right,   {6, 1} },
    movement_key_test_data{ {23, 1}, keypress_cursor_right,   {0, 2} },

    movement_key_test_data{ {0,  6}, keypress_cursor_right,   {0, 6} },

    // Move the cursor up from various points
    movement_key_test_data{ {0,  0}, keypress_cursor_up,      {0,  0} },
    movement_key_test_data{ {13, 0}, keypress_cursor_up,      {13, 0} },
    movement_key_test_data{ {27, 0}, keypress_cursor_up,      {27, 0} },
    
    movement_key_test_data{ {0,  1}, keypress_cursor_up,      {0,  0} },
    movement_key_test_data{ {27, 2}, keypress_cursor_up,      {22, 1} },

    movement_key_test_data{ {0,  0}, keypress_cursor_down,    {0,  1} },
    movement_key_test_data{ {15, 0}, keypress_cursor_down,    {15, 1} },

    movement_key_test_data{ {27, 0}, keypress_cursor_down,    {22, 1} },

    movement_key_test_data{ {0,  6}, keypress_cursor_down,    {0,  6} },

    // Multi-key tests

    // Move the cursor left multiple times:
    movement_key_test_data{ {0,  0}, keypress_cursor_left_2,  {0,  0} },
    movement_key_test_data{ {1,  0}, keypress_cursor_left_2,  {0,  0} },
    movement_key_test_data{ {2,  0}, keypress_cursor_left_2,  {0,  0} },

    movement_key_test_data{ {27, 0}, keypress_cursor_left_2,  {25, 0} },
    movement_key_test_data{ {0,  1}, keypress_cursor_left_2,  {26, 0} },
    movement_key_test_data{ {1,  1}, keypress_cursor_left_2,  {27, 0} },
    movement_key_test_data{ {2,  1}, keypress_cursor_left_2,  {0,  1} },

    // Move the cursor right multiple times
    movement_key_test_data{ {0,  0}, keypress_cursor_right_2, {2,  0} },

    movement_key_test_data{ {25, 0}, keypress_cursor_right_2, {27, 0} },
    movement_key_test_data{ {26, 0}, keypress_cursor_right_2, {0,  1} },
    movement_key_test_data{ {27, 0}, keypress_cursor_right_2, {1,  1} },

    // Move the cursor up multiple times
    movement_key_test_data{ {0,  0}, keypress_cursor_up_2,    {0,  0} },
    movement_key_test_data{ {15, 0}, keypress_cursor_up_2,    {15, 0} },
    movement_key_test_data{ {22, 0}, keypress_cursor_up_2,    {22, 0} },
    movement_key_test_data{ {27, 0}, keypress_cursor_up_2,    {27, 0} },

    movement_key_test_data{ {0,  1}, keypress_cursor_up_2,    {0,  0} },
    movement_key_test_data{ {15, 1}, keypress_cursor_up_2,    {15, 0} },
    movement_key_test_data{ {22, 1}, keypress_cursor_up_2,    {22, 0} },

    movement_key_test_data{ {0,  2}, keypress_cursor_up_2,    {0,  0} },
    movement_key_test_data{ {15, 2}, keypress_cursor_up_2,    {15, 0} },
    movement_key_test_data{ {22, 2}, keypress_cursor_up_2,    {22, 0} },
    movement_key_test_data{ {27, 2}, keypress_cursor_up_2,    {27, 0} },

    movement_key_test_data{ {0,  3}, keypress_cursor_up_2,    {0,  1} },
    movement_key_test_data{ {15, 3}, keypress_cursor_up_2,    {15, 1} },
    movement_key_test_data{ {22, 3}, keypress_cursor_up_2,    {22, 1} },
    movement_key_test_data{ {27, 3}, keypress_cursor_up_2,    {22, 1} },

    // Move the cursor down multiple times
    movement_key_test_data{ {0,  0}, keypress_cursor_down_2,  {0,  2} },
    movement_key_test_data{ {15, 0}, keypress_cursor_down_2,  {15, 2} },
    movement_key_test_data{ {22, 0}, keypress_cursor_down_2,  {22, 2} },
    movement_key_test_data{ {27, 0}, keypress_cursor_down_2,  {27, 2} },

    movement_key_test_data{ {0,  1}, keypress_cursor_down_2,  {0,  3} },
    movement_key_test_data{ {15, 1}, keypress_cursor_down_2,  {15, 3} },
    movement_key_test_data{ {22, 1}, keypress_cursor_down_2,  {22, 3} },

    // Move the cursor home
    movement_key_test_data{ {0,  0}, keypress_home,           {0,  0} },
    movement_key_test_data{ {10, 0}, keypress_home,           {0,  0} },
    movement_key_test_data{ {27, 0}, keypress_home,           {0,  0} },

    movement_key_test_data{ {0,  1}, keypress_home,           {0,  1} },
    movement_key_test_data{ {10, 1}, keypress_home,           {0,  1} },
    movement_key_test_data{ {22, 1}, keypress_home,           {0,  1} },
};

}

INSTANTIATE_TEST_SUITE_P(
    moves_the_cursor_appropriately,
    pushing_a_movement_key,
    ValuesIn(move_key_test_entries)
);
