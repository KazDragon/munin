#include "mock/graphics.hpp"
#include <munin/solid_frame.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/canvas_view.hpp>
#include <terminalpp/string.hpp>
#include <gtest/gtest.h>

using testing::NiceMock;
using testing::Return;

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

class a_solid_frame_with_unicode_graphics : public testing::Test
{
protected:
    void SetUp() override
    {
        ON_CALL(mock_gr_, supports_unicode())
            .WillByDefault(Return(true));

        munin::set_graphics(&mock_gr_);
    }

    void TearDown() override
    {
        munin::set_graphics(nullptr);
    }

    NiceMock<mock_graphics> mock_gr_;  
};

TEST_F(a_solid_frame_with_unicode_graphics, draws_a_border_with_box_drawing_glyphs)
{
    munin::solid_frame frame;
    frame.set_size({4, 4});
    
    terminalpp::canvas canvas({4, 4});
    terminalpp::canvas_view cv(canvas);
    frame.draw(cv, {{}, {4, 4}});

    using namespace terminalpp::literals;
    static terminalpp::element const top_left_corner     = "\\U2554"_ets[0];
    static terminalpp::element const top_right_corner    = "\\U2557"_ets[0];
    static terminalpp::element const bottom_left_corner  = "\\U255A"_ets[0];
    static terminalpp::element const bottom_right_corner = "\\U255D"_ets[0];
    static terminalpp::element const horizontal_beam     = "\\U2550"_ets[0];
    static terminalpp::element const vertical_beam       = "\\U2551"_ets[0];

    ASSERT_EQ(top_left_corner,     terminalpp::element(cv[0][0]));
    ASSERT_EQ(horizontal_beam,     cv[1][0]);
    ASSERT_EQ(horizontal_beam,     cv[2][0]);
    ASSERT_EQ(top_right_corner,    cv[3][0]);
    ASSERT_EQ(vertical_beam,       cv[0][1]);
    ASSERT_EQ(vertical_beam,       cv[0][2]);
    ASSERT_EQ(vertical_beam,       cv[3][2]);
    ASSERT_EQ(vertical_beam,       cv[3][1]);
    ASSERT_EQ(bottom_left_corner,  cv[0][3]);
    ASSERT_EQ(horizontal_beam,     cv[1][3]);
    ASSERT_EQ(horizontal_beam,     cv[2][3]);
    ASSERT_EQ(bottom_right_corner, cv[3][3]);
}

/*
TEST(a_solid_frame, draws_a_border)
{
}
*/