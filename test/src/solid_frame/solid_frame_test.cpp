#include "mock/component.hpp"
#include "mock/render_surface_capabilities.hpp"
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

class a_solid_frame_with_no_unicode_support : public testing::Test
{
protected :
    void SetUp() override
    {
        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(false));
    }

    NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::solid_frame frame_;
};

using a_solid_frame = a_solid_frame_with_no_unicode_support;

class a_solid_frame_with_unicode_support : public testing::Test
{
protected :
    void SetUp() override
    {
        ON_CALL(surface_capabilities_, supports_unicode())
            .WillByDefault(Return(true));
    }

    NiceMock<mock_render_surface_capabilities> surface_capabilities_;
    munin::solid_frame frame_;
};

TEST_F(a_solid_frame, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_solid_frame();
}

TEST_F(a_solid_frame_with_no_unicode_support, draws_a_border)
{
    frame_.set_size({4, 4});
    
    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

TEST_F(a_solid_frame_with_unicode_support, draws_a_border_with_box_drawing_glyphs)
{
    frame_.set_size({4, 4});
    
    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

TEST_F(a_solid_frame, can_be_displayed_with_a_custom_lowlight)
{
    static auto const lowlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    frame_.set_size({4, 4});
    frame_.set_lowlight_attribute(lowlight_attribute);
    
    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

class a_solid_frame_with_an_associated_unfocussed_component : public a_solid_frame_with_no_unicode_support
{
protected :
    void SetUp() override
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus())
            .WillByDefault(Return(false));
    }
    
    std::shared_ptr<mock_component> comp_ = make_mock_component();
};

class a_solid_frame_with_an_associated_focussed_component : public a_solid_frame_with_no_unicode_support
{
protected :
    void SetUp() override
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus())
            .WillByDefault(Return(true));
    }
    
    std::shared_ptr<mock_component> comp_ = make_mock_component();
};

TEST_F(a_solid_frame_with_an_associated_unfocussed_component, draws_a_lowlighted_border)
{
    frame_.set_size({4, 4});

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

TEST_F(a_solid_frame_with_an_associated_unfocussed_component, when_focussed_draws_a_highlighted_border)
{
    frame_.set_size({4, 4});

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

TEST_F(a_solid_frame_with_an_associated_focussed_component, when_unfocussed_draws_a_lowlit_border)
{
    frame_.set_size({4, 4});

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(false));
    comp_->on_focus_lost();

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

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

TEST_F(a_solid_frame_with_an_associated_focussed_component, can_have_a_custom_highlight)
{
    terminalpp::attribute custom_highlight = {
        terminalpp::ansi::graphics::colour::green,
        terminalpp::ansi::graphics::colour::magenta
    };
    
    frame_.set_highlight_attribute(custom_highlight);
    frame_.set_size({4, 4});

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas({4, 4});
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, {4, 4}});

    ASSERT_EQ(terminalpp::element(top_left_corner,     custom_highlight), canvas[0][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     custom_highlight), canvas[1][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     custom_highlight), canvas[2][0]);
    ASSERT_EQ(terminalpp::element(top_right_corner,    custom_highlight), canvas[3][0]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       custom_highlight), canvas[3][2]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       custom_highlight), canvas[0][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       custom_highlight), canvas[3][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam,       custom_highlight), canvas[0][2]);
    ASSERT_EQ(terminalpp::element(bottom_left_corner,  custom_highlight), canvas[0][3]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     custom_highlight), canvas[1][3]);
    ASSERT_EQ(terminalpp::element(horizontal_beam,     custom_highlight), canvas[2][3]);
    ASSERT_EQ(terminalpp::element(bottom_right_corner, custom_highlight), canvas[3][3]);
}

