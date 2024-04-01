#include "text_area_test.hpp"

#include "fill_canvas.hpp"

#include <terminalpp/mouse.hpp>

terminalpp::element const a_text_area_base::fill = {'X'};

void a_text_area_base::fill_canvas(terminalpp::extent canvas_size)
{
    canvas_ = terminalpp::canvas{canvas_size};
    ::fill_canvas(canvas_, fill);
}

TEST_F(a_text_area, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_text_area();
}

TEST_F(a_text_area, gains_focus_when_clicked)
{
    terminalpp::mouse::event mouse_down{
        terminalpp::mouse::event_type::left_button_down, {0, 0}
    };

    bool has_focus = false;
    text_area_.on_focus_set.connect([&has_focus]() { has_focus = true; });

    text_area_.event(mouse_down);
    ASSERT_TRUE(has_focus);
}
