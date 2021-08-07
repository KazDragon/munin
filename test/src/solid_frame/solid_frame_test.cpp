#include "solid_frame_test.hpp"
#include "mock/component.hpp"
#include "redraw.hpp"
#include <terminalpp/string.hpp>

using testing::NiceMock;
using testing::Return;

using namespace terminalpp::literals;
static terminalpp::glyph const top_left_corner     = '+';
static terminalpp::glyph const top_right_corner    = '+';
static terminalpp::glyph const bottom_left_corner  = '+';
static terminalpp::glyph const bottom_right_corner = '+';
static terminalpp::glyph const horizontal_beam     = '-';
static terminalpp::glyph const vertical_beam       = '|';

static terminalpp::glyph const unicode_top_left_corner     = u8"\\U256D"_ets[0].glyph_;
static terminalpp::glyph const unicode_top_right_corner    = u8"\\U256E"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_left_corner  = u8"\\U2570"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_right_corner = u8"\\U256F"_ets[0].glyph_;
static terminalpp::glyph const unicode_horizontal_beam     = u8"\\U2500"_ets[0].glyph_;
static terminalpp::glyph const unicode_vertical_beam       = u8"\\U2502"_ets[0].glyph_;

static auto const highlight_attribute = terminalpp::attribute(
    terminalpp::graphics::colour::cyan,
    terminalpp::colour(),
    terminalpp::graphics::intensity::bold);

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
        terminalpp::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::graphics::intensity::bold);
        
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
    a_solid_frame_with_an_associated_unfocussed_component()
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
    a_solid_frame_with_an_associated_focussed_component()
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus())
            .WillByDefault(Return(true));
        comp_->on_focus_set();
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

TEST_F(a_solid_frame_with_an_associated_unfocussed_component, redraws_when_associated_component_gains_focus)
{
    frame_.set_size({4, 4});
    
    int redraw_count = 0;
    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(
        [&redraw_count, &redraw_regions](auto const &regions)
        {
            ++redraw_count;
            redraw_regions = regions;
        });
    
    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();
    
    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(a_solid_frame_with_an_associated_unfocussed_component, redraws_a_reduced_amount_when_associated_component_gains_focus_when_small)
{
    frame_.set_size({1, 1});
    
    int redraw_count = 0;
    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(
        [&redraw_count, &redraw_regions](auto const &regions)
        {
            ++redraw_count;
            redraw_regions = regions;
        });
    
    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();
    
    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {1, 1}},
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
    
    for (auto region : redraw_regions)
    {
        ASSERT_GE(region.origin_.x_, 0);
        ASSERT_GE(region.origin_.y_, 0);
        ASSERT_GE(region.size_.width_, 0);
        ASSERT_GE(region.size_.height_, 0);
    }
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
        terminalpp::graphics::colour::green,
        terminalpp::graphics::colour::magenta
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

TEST_F(a_solid_frame_with_an_associated_focussed_component, redraws_when_associated_component_loses_focus)
{
    frame_.set_size({4, 4});
    
    int redraw_count = 0;
    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(
        [&redraw_count, &redraw_regions](auto const &regions)
        {
            ++redraw_count;
            redraw_regions = regions;
        });
    
    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(false));
    comp_->on_focus_lost();
    
    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
      terminalpp::rectangle{{0, 0}, {4, 1}},
      terminalpp::rectangle{{0, 0}, {1, 4}},
      terminalpp::rectangle{{3, 0}, {1, 4}},
      terminalpp::rectangle{{0, 3}, {4, 1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
