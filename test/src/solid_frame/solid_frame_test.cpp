#include "mock/component.hpp"
#include "mock/graphics.hpp"
#include <munin/solid_frame.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/string.hpp>
#include <gtest/gtest.h>

using testing::NiceMock;
using testing::Return;

using namespace terminalpp::literals;
static terminalpp::glyph const top_left_corner     = '+';
static terminalpp::glyph const top_right_corner    = '+';
static terminalpp::glyph const bottom_left_corner  = '+';
static terminalpp::glyph const bottom_right_corner = '+';
static terminalpp::glyph const horizontal_beam     = '-';
static terminalpp::glyph const vertical_beam       = '|';

static terminalpp::glyph const unicode_top_left_corner     = "\\U2554"_ets[0].glyph_;
static terminalpp::glyph const unicode_top_right_corner    = "\\U2557"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_left_corner  = "\\U255A"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_right_corner = "\\U255D"_ets[0].glyph_;
static terminalpp::glyph const unicode_horizontal_beam     = "\\U2550"_ets[0].glyph_;
static terminalpp::glyph const unicode_vertical_beam       = "\\U2551"_ets[0].glyph_;

static auto const highlight_attribute = terminalpp::attribute(
    terminalpp::ansi::graphics::colour::cyan,
    terminalpp::colour(),
    terminalpp::ansi::graphics::intensity::bold);

TEST(a_solid_frame, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_solid_frame();
}

TEST(a_solid_frame, draws_a_border)
{
    munin::solid_frame frame;
    frame.set_size({4, 4});
    
    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(horizontal_beam,     canvas[2][0]);
    ASSERT_EQ(top_right_corner,    canvas[3][0]);
    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[3][1]);
    ASSERT_EQ(vertical_beam,       canvas[0][2]);
    ASSERT_EQ(vertical_beam,       canvas[3][2]);
    ASSERT_EQ(bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(horizontal_beam,     canvas[1][3]);
    ASSERT_EQ(horizontal_beam,     canvas[2][3]);
    ASSERT_EQ(bottom_right_corner, canvas[3][3]);
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
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(unicode_top_left_corner,     canvas[0][0]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[2][0]);
    ASSERT_EQ(unicode_top_right_corner,    canvas[3][0]);
    ASSERT_EQ(unicode_vertical_beam,       canvas[0][1]);
    ASSERT_EQ(unicode_vertical_beam,       canvas[0][2]);
    ASSERT_EQ(unicode_vertical_beam,       canvas[3][2]);
    ASSERT_EQ(unicode_vertical_beam,       canvas[3][1]);
    ASSERT_EQ(unicode_bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[1][3]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[2][3]);
    ASSERT_EQ(unicode_bottom_right_corner, canvas[3][3]);
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
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), canvas[0][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), canvas[1][0]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), canvas[2][0]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), canvas[3][0]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), canvas[0][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), canvas[3][1]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), canvas[0][2]);
    ASSERT_EQ(terminalpp::element('|', lowlight_attribute), canvas[3][2]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), canvas[0][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), canvas[1][3]);
    ASSERT_EQ(terminalpp::element('-', lowlight_attribute), canvas[2][3]);
    ASSERT_EQ(terminalpp::element('+', lowlight_attribute), canvas[3][3]);
}

TEST(a_solid_frame_with_an_associated_unfocussed_component, draws_a_highlighted_border)
{
    auto comp = make_mock_component();
    ON_CALL(*comp, do_has_focus())
        .WillByDefault(Return(false));

    munin::solid_frame frame;
    frame.set_highlight_attribute(highlight_attribute);
    frame.highlight_on_focus(comp);
    frame.set_size({4, 4});

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(horizontal_beam,     canvas[2][0]);
    ASSERT_EQ(top_right_corner,    canvas[3][0]);
    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[3][1]);
    ASSERT_EQ(vertical_beam,       canvas[0][2]);
    ASSERT_EQ(vertical_beam,       canvas[3][2]);
    ASSERT_EQ(bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(horizontal_beam,     canvas[1][3]);
    ASSERT_EQ(horizontal_beam,     canvas[2][3]);
    ASSERT_EQ(bottom_right_corner, canvas[3][3]);
}

TEST(a_solid_frame_with_an_associated_unfocussed_component, when_focussed_draws_a_highlighted_border)
{
    auto comp = make_mock_component();
    ON_CALL(*comp, do_has_focus())
        .WillByDefault(Return(false));

    munin::solid_frame frame;
    frame.set_highlight_attribute(highlight_attribute);
    frame.highlight_on_focus(comp);
    frame.set_size({4, 4});

    ON_CALL(*comp, do_has_focus())
        .WillByDefault(Return(true));
    comp->on_focus_set();

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element(top_left_corner,     highlight_attribute), canvas[0][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     highlight_attribute), canvas[1][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     highlight_attribute), canvas[2][0]);
    ASSERT_EQ(terminalpp::element(top_right_corner,    highlight_attribute), canvas[3][0]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       highlight_attribute), canvas[3][2]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       highlight_attribute), canvas[0][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       highlight_attribute), canvas[3][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       highlight_attribute), canvas[0][2]);
    ASSERT_EQ(terminalpp::element(bottom_left_corner,  highlight_attribute), canvas[0][3]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     highlight_attribute), canvas[1][3]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     highlight_attribute), canvas[2][3]);
    ASSERT_EQ(terminalpp::element(bottom_right_corner, highlight_attribute), canvas[3][3]);
}

TEST(a_solid_frame_with_an_associated_focussed_component, when_unfocussed_draws_a_border)
{
    auto comp = make_mock_component();
    ON_CALL(*comp, do_has_focus())
        .WillByDefault(Return(true));

    munin::solid_frame frame;
    frame.set_highlight_attribute(highlight_attribute);
    frame.highlight_on_focus(comp);
    frame.set_size({4, 4});

    ON_CALL(*comp, do_has_focus())
        .WillByDefault(Return(false));
    comp->on_focus_lost();

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas};
    frame.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(horizontal_beam,     canvas[2][0]);
    ASSERT_EQ(top_right_corner,    canvas[3][0]);
    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[3][1]);
    ASSERT_EQ(vertical_beam,       canvas[0][2]);
    ASSERT_EQ(vertical_beam,       canvas[3][2]);
    ASSERT_EQ(bottom_left_corner,  canvas[0][3]);
    ASSERT_EQ(horizontal_beam,     canvas[1][3]);
    ASSERT_EQ(horizontal_beam,     canvas[2][3]);
    ASSERT_EQ(bottom_right_corner, canvas[3][3]);
}
