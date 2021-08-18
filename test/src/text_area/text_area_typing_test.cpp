#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/virtual_key.hpp>

using namespace terminalpp::literals;
using testing::ValuesIn;

using keypress_test_data = std::tuple<
    terminalpp::string,      // inserted text
    terminalpp::point,       // cursor position
    terminalpp::virtual_key, // keypress
    terminalpp::string,      // expected text
    terminalpp::point        // expected cursor position
>;

namespace {

class a_text_area_receiving_keypresses
  : public a_text_area_base,
    public testing::TestWithParam<keypress_test_data>
{
};

auto const keypress_a = terminalpp::virtual_key {
    terminalpp::vk::lowercase_a,
    terminalpp::vk_modifier::none,
    1
};

auto const keypress_b = terminalpp::virtual_key {
    terminalpp::vk::uppercase_b,
    terminalpp::vk_modifier::none,
    1,
};

auto const keypress_c = terminalpp::virtual_key {
    terminalpp::vk::uppercase_c,
    terminalpp::vk_modifier::none,
    1,
};

auto const keypress_bs = terminalpp::virtual_key {
    terminalpp::vk::bs,
    terminalpp::vk_modifier::none,
    1
};

auto const keypress_del = terminalpp::virtual_key {
    terminalpp::vk::del,
    terminalpp::vk_modifier::none,
    1
};

}

TEST_P(a_text_area_receiving_keypresses, responds_in_the_specified_manner)
{
    using std::get;

    auto const &params = GetParam();
    auto const &inserted_text = get<0>(params);
    auto const &cursor_position = get<1>(params);
    auto const &keypress = get<2>(params);
    auto const &expected_text = get<3>(params);
    auto const &expected_cursor_position = get<4>(params);

    terminalpp::canvas cvs({10, 1});
    munin::render_surface surface{cvs};

    text_area_.on_redraw.connect(
        [&surface, this](auto const &regions)
        {
            for (auto const &region : regions)
            {
                text_area_.draw(surface, region);
            }
        });

    text_area_.set_size({10, 1});
    text_area_.insert_text(inserted_text);
    text_area_.set_cursor_position(cursor_position);

    auto resultant_cursor_position = cursor_position;
    text_area_.on_cursor_position_changed.connect(
        [&resultant_cursor_position, this]
        {
            resultant_cursor_position = text_area_.get_cursor_position();
        });
        
    text_area_.event(keypress);
    
    auto const &text = text_area_.get_text();
    EXPECT_EQ(expected_text, text);
    EXPECT_EQ(expected_cursor_position, resultant_cursor_position);

    for (terminalpp::coordinate_type col = 0; col < cvs.size().width_; ++col)
    {
        if (col < text.size())
        {
            EXPECT_EQ(text[col], cvs[col][0]) 
                << "at column " << col;
        }
        else
        {
            EXPECT_EQ(terminalpp::element{' '}, cvs[col][0]) 
                << "at column " << col;
        }
    }
}

static keypress_test_data const keypress_data[] = 
{
    { ""_ts,     {0, 0}, keypress_a,   "a"_ts,   {1, 0} },
    { "a"_ts,    {1, 0}, keypress_b,   "aB"_ts,  {2, 0} },
    { "aB"_ts,   {0, 0}, keypress_c,   "CaB"_ts, {1, 0} },

    { "test"_ts, {4, 0}, keypress_bs,  "tes"_ts, {3, 0} },
    { "test"_ts, {4, 0}, keypress_del, "tes"_ts, {3, 0} },
};

INSTANTIATE_TEST_SUITE_P(
    text_areas_handle_keypresses,
    a_text_area_receiving_keypresses,
    ValuesIn(keypress_data)
);
