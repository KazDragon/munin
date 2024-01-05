#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/virtual_key.hpp>

using namespace terminalpp::literals;
using testing::ValuesIn;

using keypress_test_data = std::tuple<
    terminalpp::string,       // inserted text
    terminalpp::point,        // cursor position
    terminalpp::virtual_key,  // keypress
    terminalpp::string,       // expected text
    terminalpp::point         // expected cursor position
    >;

namespace {

class a_text_area_receiving_keypresses
  : public a_text_area_base,
    public testing::TestWithParam<keypress_test_data>
{
 public:
  a_text_area_receiving_keypresses()
  {
    using std::get;

    text_area_.on_redraw.connect(
        [this](auto const &regions)
        {
          for (auto const &region : regions)
          {
            text_area_.draw(surface_, region);
          }
        });

    text_area_.on_cursor_position_changed.connect(
        [this] { cursor_position_ = text_area_.get_cursor_position(); });

    auto const &params = GetParam();
    auto const &inserted_text = get<0>(params);
    auto const &cursor_position = get<1>(params);

    text_area_.set_size({10, 3});
    text_area_.insert_text(inserted_text);
    text_area_.set_cursor_position(cursor_position);
  }

 protected:
  terminalpp::canvas canvas_{{10, 3}};
  munin::render_surface surface_{canvas_};

  terminalpp::point cursor_position_;
};

auto const keypress_a = terminalpp::virtual_key{
    terminalpp::vk::lowercase_a, terminalpp::vk_modifier::none, 1};

auto const keypress_b = terminalpp::virtual_key{
    terminalpp::vk::uppercase_b,
    terminalpp::vk_modifier::none,
    1,
};

auto const keypress_c = terminalpp::virtual_key{
    terminalpp::vk::uppercase_c,
    terminalpp::vk_modifier::none,
    1,
};

auto const keypress_bs = terminalpp::virtual_key{
    terminalpp::vk::bs, terminalpp::vk_modifier::none, 1};

auto const keypress_del = terminalpp::virtual_key{
    terminalpp::vk::del, terminalpp::vk_modifier::none, 1};

auto const keypress_lf = terminalpp::virtual_key{
    terminalpp::vk::lf, terminalpp::vk_modifier::none, 1};

auto const keypress_cr = terminalpp::virtual_key{
    terminalpp::vk::cr, terminalpp::vk_modifier::none, 1};

auto const keypress_enter = terminalpp::virtual_key{
    terminalpp::vk::enter, terminalpp::vk_modifier::none, 1};

}  // namespace

TEST_P(a_text_area_receiving_keypresses, leaves_the_cursor_in_the_correct_place)
{
  using std::get;

  auto const &params = GetParam();
  auto const &keypress = get<2>(params);
  auto const &expected_cursor_position = get<4>(params);

  text_area_.event(keypress);

  EXPECT_EQ(expected_cursor_position, cursor_position_);
}

TEST_P(a_text_area_receiving_keypresses, redraws_the_text_area_appropriately)
{
  using std::get;

  auto const &params = GetParam();
  auto const &keypress = get<2>(params);
  auto const &expected_text = get<3>(params);

  text_area_.event(keypress);

  auto const &text = text_area_.get_text();
  EXPECT_EQ(expected_text, text);

  auto const &expected_content = [&text]
  {
    std::vector<terminalpp::string> content;
    content.push_back({});

    for (auto const &elem : text)
    {
      if (elem == '\n')
      {
        content.push_back({});
      }
      else
      {
        content.back() += elem;
      }
    }

    return content;
  }();

  terminalpp::for_each_in_region(
      canvas_,
      {{}, canvas_.size()},
      [&](terminalpp::element const &elem,
          terminalpp::coordinate_type col,
          terminalpp::coordinate_type row)
      {
        if (row + 1 > expected_content.size()
            || col + 1 > expected_content[row].size())
        {
          EXPECT_EQ(' ', elem) << "at [" << col << "][" << row << "]";
        }
        else
        {
          EXPECT_EQ(expected_content[row][col], elem)
              << "at [" << col << "][" << row << "]";
        }
      });
}

static keypress_test_data const keypress_data[] = {
    {""_ts, {0, 0}, keypress_a, "a"_ts, {1, 0}},
    {"a"_ts, {1, 0}, keypress_b, "aB"_ts, {2, 0}},
    {"aB"_ts, {0, 0}, keypress_c, "CaB"_ts, {1, 0}},

    {"test"_ts, {4, 0}, keypress_bs, "tes"_ts, {3, 0}},
    {"test"_ts, {4, 0}, keypress_del, "tes"_ts, {3, 0}},

    {"test"_ts, {0, 0}, keypress_bs, "test"_ts, {0, 0}},
    {"test"_ts, {0, 0}, keypress_del, "test"_ts, {0, 0}},

    {"test"_ts, {4, 0}, keypress_cr, "test"_ts, {4, 0}},
    {"test"_ts, {4, 0}, keypress_lf, "test"_ts, {4, 0}},
    {"test"_ts, {4, 0}, keypress_enter, "test\n"_ts, {0, 1}},

    {"test"_ts, {2, 0}, keypress_enter, "te\nst"_ts, {0, 1}},

    {"te\nst"_ts, {0, 1}, keypress_bs, "test"_ts, {2, 0}},
};

INSTANTIATE_TEST_SUITE_P(
    text_areas_handle_keypresses,
    a_text_area_receiving_keypresses,
    ValuesIn(keypress_data));
