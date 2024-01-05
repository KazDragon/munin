#include "fill_canvas.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/virtual_key.hpp>
#include <gtest/gtest.h>

using testing::ValuesIn;

namespace {

using keypress_data = std::tuple<
    terminalpp::string,          // beginning content
    terminalpp::point,           // initial cursor position
    terminalpp::vk,              // virtual keypress
    terminalpp::string,          // expected text drawn
    terminalpp::string,          // expected text
    terminalpp::point,           // expected final cursor position
    terminalpp::coordinate_type  // expected final caret position
    >;

class keypress_test : public testing::TestWithParam<keypress_data>
{
 protected:
  keypress_test()
  {
    fill_canvas(cvs_, 'x');
    surface_.offset_by({1, 1});

    edit_.on_redraw.connect(
        [this](auto const &regions)
        {
          for (auto const &region : regions)
          {
            edit_.draw(surface_, region);
          }
        });

    edit_.set_position({0, 0});
    edit_.set_size({4, 1});

    edit_.on_redraw({{{0, 0}, edit_.get_size()}});
  }

  terminalpp::canvas cvs_{{6, 3}};
  munin::render_surface surface_{cvs_};
  munin::edit edit_;
};

}  // namespace

TEST_P(keypress_test, when_an_edit_with_content_receives_a_keypress)
{
  using std::get;

  auto const &params = GetParam();
  auto const &initial_content = get<0>(params);
  auto const &initial_pos = get<1>(params);
  auto const &keypress = get<2>(params);
  auto const &expected_content = get<3>(params);
  auto const &expected_text = get<4>(params);
  auto const &expected_pos = get<5>(params);
  auto const &expected_caret_pos = get<6>(params);

  edit_.insert_text(initial_content);
  edit_.set_cursor_position(initial_pos);
  assert(edit_.get_caret_position() == initial_pos.x_);

  edit_.event(terminalpp::virtual_key{keypress});

  EXPECT_EQ(expected_pos, edit_.get_cursor_position());
  EXPECT_EQ(expected_caret_pos, edit_.get_caret_position());

  terminalpp::for_each_in_region(
      cvs_,
      {{}, cvs_.size()},
      [&](terminalpp::element &elem,
          terminalpp::coordinate_type column,
          terminalpp::coordinate_type row)
      {
        if (row == 1)
        {
          if (column == 0)
          {
            EXPECT_EQ('x', cvs_[column][row])
                << "at position [" << column << "," << row << "]";
          }
          else if (column - 1 < expected_content.size())
          {
            EXPECT_EQ(expected_content[column - 1], cvs_[column][row])
                << "at position [" << column << "," << row << "]";
          }
          else if (column - 1 < edit_.get_size().width_)
          {
            EXPECT_EQ(' ', cvs_[column][row])
                << "at position [" << column << "," << row << "]";
          }
          else
          {
            EXPECT_EQ('x', cvs_[column][row])
                << "at position [" << column << "," << row << "]";
          }
        }
        else
        {
          EXPECT_EQ('x', cvs_[column][row])
              << "at position [" << column << "," << row << "]";
        }
      });

  EXPECT_EQ(expected_text, edit_.get_text());
}

INSTANTIATE_TEST_SUITE_P(
    keypresses,
    keypress_test,
    ValuesIn({
        // clang-format off
        // Cases with only one character in the content.
        keypress_data{ "t", {1, 0}, terminalpp::vk::lowercase_e, "te", "te", {2, 0}, 2 },
        keypress_data{ "t", {1, 0}, terminalpp::vk::uppercase_u, "tU", "tU", {2, 0}, 2 },
        
        // Cases with multiple characters in the content.
        keypress_data{ "tes", {0, 0}, terminalpp::vk::uppercase_t, "Ttes", "Ttes", {1, 0}, 1 },
        keypress_data{ "tes", {1, 0}, terminalpp::vk::uppercase_t, "tTes", "tTes", {2, 0}, 2 },
        keypress_data{ "tes", {2, 0}, terminalpp::vk::uppercase_t, "teTs", "teTs", {3, 0}, 3 },
        keypress_data{ "tes", {3, 0}, terminalpp::vk::uppercase_t, "tesT", "tesT", {3, 0}, 4 },

        // Cursor movement.
        keypress_data{ "t", {1, 0}, terminalpp::vk::cursor_right, "t", "t", {1, 0}, 1 },
        keypress_data{ "t", {1, 0}, terminalpp::vk::cursor_left,  "t", "t", {0, 0}, 0 },
        keypress_data{ "t", {0, 0}, terminalpp::vk::cursor_right, "t", "t", {1, 0}, 1 },
        keypress_data{ "t", {0, 0}, terminalpp::vk::cursor_left,  "t", "t", {0, 0}, 0 },
        
        // Home/end keys
        keypress_data{ "t",    {0, 0}, terminalpp::vk::home, "t",    "t", {0, 0}, 0 },
        keypress_data{ "t",    {1, 0}, terminalpp::vk::home, "t",    "t", {0, 0}, 0 },
        keypress_data{ "test", {0, 0}, terminalpp::vk::home, "test", "test", {0, 0}, 0 },
        keypress_data{ "test", {1, 0}, terminalpp::vk::home, "test", "test", {0, 0}, 0 },
        keypress_data{ "test", {2, 0}, terminalpp::vk::home, "test", "test", {0, 0}, 0 },
        keypress_data{ "test", {3, 0}, terminalpp::vk::home, "test", "test", {0, 0}, 0 },
        keypress_data{ "test", {4, 0}, terminalpp::vk::home, "test", "test", {0, 0}, 0 },

        keypress_data{ "t",    {0, 0}, terminalpp::vk::end, "t",    "t",    {1, 0}, 1 },
        keypress_data{ "t",    {1, 0}, terminalpp::vk::end, "t",    "t",    {1, 0}, 1 },
        keypress_data{ "test", {0, 0}, terminalpp::vk::end, "test", "test", {3, 0}, 4 },
        keypress_data{ "test", {1, 0}, terminalpp::vk::end, "test", "test", {3, 0}, 4 },
        keypress_data{ "test", {2, 0}, terminalpp::vk::end, "test", "test", {3, 0}, 4 },
        keypress_data{ "test", {3, 0}, terminalpp::vk::end, "test", "test", {3, 0}, 4 },
        keypress_data{ "test", {4, 0}, terminalpp::vk::end, "test", "test", {3, 0}, 4 },

        // Test writing off the end of the viewable portion of the edit.
        // In all cases, this should continue exactly as it did, but should
        // not be drawn (it's the job of a Viewport to have a view that follows
        // the cursor)
        //
        // Note: an edit in this test is 4x1.
        keypress_data{ "test", {0, 0}, terminalpp::vk::lowercase_z, "ztes", "ztest", {1, 0}, 1 },
        keypress_data{ "test", {1, 0}, terminalpp::vk::lowercase_z, "tzes", "tzest", {2, 0}, 2 },
        keypress_data{ "test", {2, 0}, terminalpp::vk::lowercase_z, "tezs", "tezst", {3, 0}, 3 },
        keypress_data{ "test", {3, 0}, terminalpp::vk::lowercase_z, "tesz", "teszt", {3, 0}, 4 },
        keypress_data{ "test", {4, 0}, terminalpp::vk::lowercase_z, "test", "testz", {3, 0}, 5 },
        
        // Pressing the backspace or DEL keys result in deleting the character
        // to the left of the cursor and retreating the cursor one step.
        keypress_data{ "test", {4, 0}, terminalpp::vk::bs, "tes",  "tes",  {3, 0}, 3 },
        keypress_data{ "test", {3, 0}, terminalpp::vk::bs, "tet",  "tet",  {2, 0}, 2 },
        keypress_data{ "test", {2, 0}, terminalpp::vk::bs, "tst",  "tst",  {1, 0}, 1 },
        keypress_data{ "test", {1, 0}, terminalpp::vk::bs, "est",  "est",  {0, 0}, 0 },
        keypress_data{ "test", {0, 0}, terminalpp::vk::bs, "test", "test", {0, 0}, 0 },

        keypress_data{ "test", {4, 0}, terminalpp::vk::del, "tes",  "tes",  {3, 0}, 3 },
        keypress_data{ "test", {3, 0}, terminalpp::vk::del, "tet",  "tet",  {2, 0}, 2 },
        keypress_data{ "test", {2, 0}, terminalpp::vk::del, "tst",  "tst",  {1, 0}, 1 },
        keypress_data{ "test", {1, 0}, terminalpp::vk::del, "est",  "est",  {0, 0}, 0 },
        keypress_data{ "test", {0, 0}, terminalpp::vk::del, "test", "test", {0, 0}, 0 },
        // clang-format on
    }));
