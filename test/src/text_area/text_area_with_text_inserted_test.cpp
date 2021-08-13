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
    munin::text_area::text_index, // expected caret position
    terminalpp::point             // expected cursor position
>;

class a_text_area_with_many_lines_of_text_inserted_base
  : public a_text_area_base
{
public:
    a_text_area_with_many_lines_of_text_inserted_base()
    {
        text_area_.insert_text(
          // 0         1         2         3
          // 0123456789012345678901234567890123456789
            "Lorem ipsum dolor sit amet,\n"          // 0  + 28 = 28
            "consectetur adipiscing\n"               // 28 + 23 = 51
            "elit. Proin sed nisl mattis,\n"         // 51 + 29 = 80
            "luctus velit ullamcorper, molestie\n"   // 80 + 35 = 115
            "mauris.\n");                            // 115 + 8 = 123

        // Set the size of the text area so that the width is precisely on
        // a newline boundary.
        text_area_.set_size({29, 10}); // See "elit.  Proin sed nisl" line.
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
    auto const &expected_caret_position = get<1>(params);
    auto const &expected_cursor_position = get<2>(params);

    auto cursor_position = text_area_.get_cursor_position();
    text_area_.on_cursor_position_changed.connect(
        [this, &cursor_position]
        {
            cursor_position = text_area_.get_cursor_position();
        });

    text_area_.set_caret_position(caret_position);

    EXPECT_EQ(expected_caret_position, text_area_.get_caret_position());
    EXPECT_EQ(expected_cursor_position, cursor_position);
}

static move_caret_test_data const move_caret_test_entries[] =
{
    // Move to the beginning of the text
    move_caret_test_data{ 0,  0,    {0,  0} },

    // Move to the end of the first line and the beginning of the next 
    // wrapped as determined by the newline.
    move_caret_test_data{ 26,  26,  {26, 0} }, // t
    move_caret_test_data{ 27,  27,  {27, 0} }, // ,
    move_caret_test_data{ 28,  28,  {0,  1} }, // newline
    move_caret_test_data{ 29,  29,  {1,  1} }, // c

    // Moving on the end of the third line, the caret has two
    // positions that match, one for writing before the newline,
    // another after it.
    move_caret_test_data{ 78,  78,  {27, 2} }, // s
    move_caret_test_data{ 79,  79,  {28, 2} }, // (wrap)
    move_caret_test_data{ 80,  80,  {28, 2} }, // \n
    move_caret_test_data{ 81,  81,  {0,  3} }, // l

    // The text wraps at the end of the fourth line, which should
    // happen smoothly.
    move_caret_test_data{ 108, 108, {27, 3} }, // o
    move_caret_test_data{ 109, 109, {28, 3} }, // l
    move_caret_test_data{ 110, 110, {0,  4} }, // e
    move_caret_test_data{ 111, 111, {1,  4} }, // s

    // Move off the end of the whole text; caret should stay at end
    // of the whole piece.
    move_caret_test_data{ 140, 123, {0,  6} },
};

INSTANTIATE_TEST_SUITE_P(
    allows_movement_of_the_caret,
    setting_the_caret_programatically,
    ValuesIn(move_caret_test_entries)
);

namespace {

using move_cursor_test_data = std::tuple<
    terminalpp::point,           // cursor set position
    terminalpp::point,           // expected cursor position
    munin::text_area::text_index // expected caret position
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

    text_area_.set_cursor_position(cursor_position);

    ASSERT_EQ(expected_cursor_position, text_area_.get_cursor_position());
    ASSERT_EQ(expected_caret_position, text_area_.get_caret_position());
}

static move_cursor_test_data const move_cursor_test_entries[] =
{
    // Move the cursor to the home point.
    move_cursor_test_data{ {0,  0}, {0,  0}, 0   },

    // Move the cursor to arbitrary positions on the first row
    move_cursor_test_data{ {2,  0}, {2,  0}, 2   },
    move_cursor_test_data{ {4,  0}, {4,  0}, 4   },
    move_cursor_test_data{ {12, 0}, {12, 0}, 12  },
    move_cursor_test_data{ {20, 0}, {20, 0}, 20  },
    move_cursor_test_data{ {27, 0}, {27, 0}, 27  },

    // Move the cursor to arbitrary positions on the second row
    move_cursor_test_data{ {0,  1}, {0,  1}, 28  },
    move_cursor_test_data{ {11, 1}, {11, 1}, 39  },
    move_cursor_test_data{ {13, 1}, {13, 1}, 41  },
    move_cursor_test_data{ {22, 1}, {22, 1}, 50  },

    // Moving the cursor off the end of text clips to the end of the line
    move_cursor_test_data{ {26, 1}, {22, 1}, 50  },
    move_cursor_test_data{ {14, 4}, {4,  4}, 114 },

    // Move the cursor of the bottom of the text clips to the end of the text
    move_cursor_test_data{ {3,  9}, {0,  6}, 123 },
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
    
    auto reported_cursor_position = terminalpp::point{};
    text_area_.on_cursor_position_changed.connect(
        [this, &reported_cursor_position]
        {
            reported_cursor_position = text_area_.get_cursor_position();
        });

    text_area_.event(keypress);

    ASSERT_EQ(expected_cursor_position, reported_cursor_position);
}

namespace {

static auto const keypress_cursor_left = terminalpp::virtual_key {
    terminalpp::vk::cursor_left,
    terminalpp::vk_modifier::none,
    1
};

static auto const keypress_cursor_right = terminalpp::virtual_key {
    terminalpp::vk::cursor_right,
    terminalpp::vk_modifier::none,
    1
};

static movement_key_test_data const move_key_test_entries[] =
{
    // Move the cursor left from various points
    movement_key_test_data{ {0,  0}, keypress_cursor_left,  {0,  0} },
    movement_key_test_data{ {1,  0}, keypress_cursor_left,  {0,  0} },
    movement_key_test_data{ {27, 0}, keypress_cursor_left,  {26, 0} },
    movement_key_test_data{ {0,  1}, keypress_cursor_left,  {27, 0} },

    // Move the cursor right from various points
    movement_key_test_data{ {0,  0}, keypress_cursor_right, {1, 0} },

    movement_key_test_data{ {0,  1}, keypress_cursor_right, {1, 1} },
    movement_key_test_data{ {5,  1}, keypress_cursor_right, {6, 1} },
    movement_key_test_data{ {23, 1}, keypress_cursor_right, {0, 2} },

    movement_key_test_data{ {0,  6}, keypress_cursor_right, {0, 6} },
};

}

INSTANTIATE_TEST_SUITE_P(
    moves_the_cursor_appropriately,
    pushing_a_movement_key,
    ValuesIn(move_key_test_entries)
);
