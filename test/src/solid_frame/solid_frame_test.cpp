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