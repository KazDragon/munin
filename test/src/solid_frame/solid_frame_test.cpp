#include <munin/solid_frame.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <gtest/gtest.h>

TEST(a_solid_frame, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_solid_frame();
}

TEST(a_solid_frame, draws_a_border)
{
    munin::solid_frame frame;
    frame.set_size({4, 4});
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element('+'), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-'), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-'), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+'), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|'), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|'), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|'), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|'), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+'), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-'), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-'), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+'), cv[3][3]);
}

TEST(a_highlighted_solid_frame, draws_a_highlighted_border)
{
    munin::solid_frame frame;
    frame.set_size({4, 4});
    frame.set_highlight(true);
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});
    
    auto const highlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);

    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][3]);
}

TEST(a_solid_frame, can_be_displayed_with_a_custom_lowlight)
{
    static auto const lowlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    munin::solid_frame frame;
    frame.set_size({4, 4});
    frame.set_lowlight_attribute(lowlight_attribute);
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});
    

    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[3][3]);
}

TEST(a_highlighted_solid_frame, remains_highlighted_when_setting_the_lowlight_attribute)
{
    static auto const lowlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    static auto const highlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::cyan,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    munin::solid_frame frame;
    frame.set_size({4, 4});
    frame.set_highlight(true);
    frame.set_lowlight_attribute(lowlight_attribute);
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});
    
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][3]);
}

TEST(a_highlighted_solid_frame, can_be_displayed_with_a_custom_highlight)
{
    static auto const highlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::magenta,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::faint);
        
    munin::solid_frame frame;
    frame.set_size({4, 4});
    frame.set_highlight(true);
    frame.set_highlight_attribute(highlight_attribute);
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|', highlight_attribute), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-', highlight_attribute), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+', highlight_attribute), cv[3][3]);
}

TEST(a_solid_frame, remains_unhighlighted_when_setting_the_highlight_attribute)
{
    static auto const lowlight_attribute = terminalpp::attribute{};
    static auto const highlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::magenta,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::faint);
        
    munin::solid_frame frame;
    frame.set_size({4, 4});
    frame.set_highlight(false);
    frame.set_highlight_attribute(highlight_attribute);
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[0][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[1][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[2][0]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[3][0]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[0][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[3][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[0][2]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), cv[3][2]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[0][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[1][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), cv[2][3]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), cv[3][3]);
}
