#include "fill_canvas.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/point.hpp>
#include <terminalpp/string.hpp>
#include <gtest/gtest.h>
#include <tuple>

using testing::ValuesIn;

namespace {

using mouse_test_data = std::tuple<
    terminalpp::string, // text content
    terminalpp::point,  // cursor position
    
    terminalpp::point,  // mouse position
    terminalpp::byte,   // mouse button
    
    terminalpp::point   // expected cursor position
>;

class mouse_click_test : public testing::TestWithParam<mouse_test_data>
{
protected:
    mouse_click_test()
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
        
        edit_.on_redraw({
            {{0, 0}, edit_.get_size()}
        });
    }
    
    terminalpp::canvas cvs_{{6, 3}};
    munin::render_surface surface_{cvs_};
    munin::edit edit_;
};

}

TEST_P(mouse_click_test, when_an_edit_receives_a_mouse_event)
{
    using std::get;
    
    auto const &params = GetParam();
    auto const &initial_content  = get<0>(params);
    auto const &initial_pos      = get<1>(params);
    auto const &mouse_position   = get<2>(params);
    auto const &mouse_button     = get<3>(params);
    auto const &expected_pos     = get<4>(params);

    edit_.insert_text(initial_content);
    edit_.set_cursor_position(initial_pos);
    assert(edit_.get_cursor_position() == initial_pos);
    
    terminalpp::ansi::mouse::report mouse_event = {
        mouse_button,
        mouse_position.x,
        mouse_position.y,
    };
    
    edit_.event(mouse_event);
    
    EXPECT_EQ(expected_pos, edit_.get_cursor_position());
}

namespace {
auto constexpr lmb = terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN;
auto constexpr rmb = terminalpp::ansi::mouse::report::RIGHT_BUTTON_DOWN;
auto constexpr mup = terminalpp::ansi::mouse::report::BUTTON_UP;
}

INSTANTIATE_TEST_SUITE_P(
    mouse_events,
    mouse_click_test,
    ValuesIn
    ({
        // If the edit is empty, then clicking anywhere does not move the
        // cursor anywhere.
        mouse_test_data{ "", {0, 0}, {0, 0}, lmb, {0, 0} },
        mouse_test_data{ "", {0, 0}, {1, 0}, lmb, {0, 0} },
        mouse_test_data{ "", {0, 0}, {2, 0}, lmb, {0, 0} },
        mouse_test_data{ "", {0, 0}, {3, 0}, lmb, {0, 0} },
        
        // With a character in the text, the cursor will move to at most the
        // empty space past the character.
        mouse_test_data{ "T", {0, 0}, {0, 0}, lmb, {0, 0} },
        mouse_test_data{ "T", {0, 0}, {1, 0}, lmb, {1, 0} },
        mouse_test_data{ "T", {0, 0}, {2, 0}, lmb, {1, 0} },
        mouse_test_data{ "T", {0, 0}, {3, 0}, lmb, {1, 0} },
        
        // With many characters, the cursor can settle on any of them.
        mouse_test_data{ "TEST", {0, 0}, {0, 0}, lmb, {0, 0} },
        mouse_test_data{ "TEST", {0, 0}, {1, 0}, lmb, {1, 0} },
        mouse_test_data{ "TEST", {0, 0}, {2, 0}, lmb, {2, 0} },
        mouse_test_data{ "TEST", {0, 0}, {3, 0}, lmb, {3, 0} },
        
        // Right mouse button and button up events are ignored.
        mouse_test_data{ "TEST", {0, 0}, {2, 0}, rmb, {0, 0} },
        mouse_test_data{ "TEST", {0, 0}, {2, 0}, mup, {0, 0} },
    })
);

TEST(an_edit, receives_focus_when_clicked)
{
    munin::edit edit;
    edit.event(terminalpp::ansi::mouse::report{
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN});
    ASSERT_TRUE(edit.has_focus());
}