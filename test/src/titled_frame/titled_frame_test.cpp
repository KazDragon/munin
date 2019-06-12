#include "titled_frame_test.hpp"
#include "mock/component.hpp"
#include "mock/render_surface_capabilities.hpp"
#include "redraw.hpp"
#include <munin/titled_frame.hpp>
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

static terminalpp::glyph const unicode_top_left_corner     = "\\U256D"_ets[0].glyph_;
static terminalpp::glyph const unicode_top_right_corner    = "\\U256E"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_left_corner  = "\\U2570"_ets[0].glyph_;
static terminalpp::glyph const unicode_bottom_right_corner = "\\U256F"_ets[0].glyph_;
static terminalpp::glyph const unicode_horizontal_beam     = "\\U2500"_ets[0].glyph_;
static terminalpp::glyph const unicode_vertical_beam       = "\\U2502"_ets[0].glyph_;

static auto const highlight_attribute = terminalpp::attribute(
    terminalpp::ansi::graphics::colour::cyan,
    terminalpp::colour(),
    terminalpp::ansi::graphics::intensity::bold);

TEST_F(a_titled_frame, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_titled_frame("title");
}

TEST_F(a_titled_frame_with_no_unicode_support, draws_a_border_with_title)
{
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                 canvas[2][0]);
    ASSERT_EQ('t',                 canvas[3][0]);
    ASSERT_EQ('i',                 canvas[4][0]);
    ASSERT_EQ('t',                 canvas[5][0]);
    ASSERT_EQ('l',                 canvas[6][0]);
    ASSERT_EQ('e',                 canvas[7][0]);
    ASSERT_EQ(' ',                 canvas[8][0]);
    ASSERT_EQ(horizontal_beam,     canvas[9][0]);
    ASSERT_EQ(top_right_corner,    canvas[10][0]);

    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[10][1]);
    
    ASSERT_EQ(bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(horizontal_beam,     canvas[8][2]);
    ASSERT_EQ(horizontal_beam,     canvas[9][2]);
    ASSERT_EQ(bottom_right_corner, canvas[10][2]);
}

TEST_F(a_titled_frame_with_no_unicode_support, left_justifies_text_when_grown)
{
    auto const size = terminalpp::extent{13, 3};
    frame_.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                 canvas[2][0]);
    ASSERT_EQ('t',                 canvas[3][0]);
    ASSERT_EQ('i',                 canvas[4][0]);
    ASSERT_EQ('t',                 canvas[5][0]);
    ASSERT_EQ('l',                 canvas[6][0]);
    ASSERT_EQ('e',                 canvas[7][0]);
    ASSERT_EQ(' ',                 canvas[8][0]);
    ASSERT_EQ(horizontal_beam,     canvas[9][0]);
    ASSERT_EQ(horizontal_beam,     canvas[10][0]);
    ASSERT_EQ(horizontal_beam,     canvas[11][0]);
    ASSERT_EQ(top_right_corner,    canvas[12][0]);

    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[12][1]);
    
    ASSERT_EQ(bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(horizontal_beam,     canvas[8][2]);
    ASSERT_EQ(horizontal_beam,     canvas[9][2]);
    ASSERT_EQ(horizontal_beam,     canvas[10][2]);
    ASSERT_EQ(horizontal_beam,     canvas[11][2]);
    ASSERT_EQ(bottom_right_corner, canvas[12][2]);
}

TEST_F(a_titled_frame_with_no_unicode_support, clips_the_title_when_shrunk)
{
    auto const size = terminalpp::extent{9, 3};
    frame_.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                 canvas[2][0]);
    ASSERT_EQ('i',                 canvas[3][0]);
    ASSERT_EQ('t',                 canvas[4][0]);
    ASSERT_EQ('l',                 canvas[5][0]);
    ASSERT_EQ(' ',                 canvas[6][0]);
    ASSERT_EQ(horizontal_beam,     canvas[7][0]);
    ASSERT_EQ(top_right_corner,    canvas[8][0]);

    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[8][1]);
    
    ASSERT_EQ(bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(bottom_right_corner, canvas[8][2]);
}

TEST_F(a_titled_frame_with_unicode_support, draws_a_border_with_box_drawing_glyphs)
{
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(unicode_top_left_corner,     canvas[0][0]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                         canvas[2][0]);
    ASSERT_EQ('t',                         canvas[3][0]);
    ASSERT_EQ('i',                         canvas[4][0]);
    ASSERT_EQ('t',                         canvas[5][0]);
    ASSERT_EQ('l',                         canvas[6][0]);
    ASSERT_EQ('e',                         canvas[7][0]);
    ASSERT_EQ(' ',                         canvas[8][0]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[9][0]);
    ASSERT_EQ(unicode_top_right_corner,    canvas[10][0]);

    ASSERT_EQ(unicode_vertical_beam,       canvas[0][1]);
    ASSERT_EQ(unicode_vertical_beam,       canvas[10][1]);
    
    ASSERT_EQ(unicode_bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[8][2]);
    ASSERT_EQ(unicode_horizontal_beam,     canvas[9][2]);
    ASSERT_EQ(unicode_bottom_right_corner, canvas[10][2]);
}

TEST_F(a_titled_frame, can_be_displayed_with_a_custom_lowlight)
{
    static auto const lowlight_attribute = terminalpp::attribute(
        terminalpp::ansi::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::ansi::graphics::intensity::bold);
        
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    frame_.set_lowlight_attribute(lowlight_attribute);
    
    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(terminalpp::element(top_left_corner, lowlight_attribute),     canvas[0][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[1][0]);
    ASSERT_EQ(' ',                                                          canvas[2][0]);
    ASSERT_EQ('t',                                                          canvas[3][0]);
    ASSERT_EQ('i',                                                          canvas[4][0]);
    ASSERT_EQ('t',                                                          canvas[5][0]);
    ASSERT_EQ('l',                                                          canvas[6][0]);
    ASSERT_EQ('e',                                                          canvas[7][0]);
    ASSERT_EQ(' ',                                                          canvas[8][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[9][0]);
    ASSERT_EQ(terminalpp::element(top_right_corner, lowlight_attribute),    canvas[10][0]);

    ASSERT_EQ(terminalpp::element(vertical_beam, lowlight_attribute),       canvas[0][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam, lowlight_attribute),       canvas[10][1]);
    
    ASSERT_EQ(terminalpp::element(bottom_left_corner, lowlight_attribute),  canvas[0][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[1][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[2][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[3][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[4][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[5][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[6][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[7][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[8][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, lowlight_attribute),     canvas[9][2]);
    ASSERT_EQ(terminalpp::element(bottom_right_corner, lowlight_attribute), canvas[10][2]);
}

class a_titled_frame_with_an_associated_unfocussed_component : public a_titled_frame_with_no_unicode_support
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

class a_titled_frame_with_an_associated_focussed_component : public a_titled_frame_with_no_unicode_support
{
protected :
    void SetUp() override
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus())
            .WillByDefault(Return(true));
        comp_->on_focus_set();
    }
    
    std::shared_ptr<mock_component> comp_ = make_mock_component();
};

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, draws_a_lowlighted_border)
{
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                 canvas[2][0]);
    ASSERT_EQ('t',                 canvas[3][0]);
    ASSERT_EQ('i',                 canvas[4][0]);
    ASSERT_EQ('t',                 canvas[5][0]);
    ASSERT_EQ('l',                 canvas[6][0]);
    ASSERT_EQ('e',                 canvas[7][0]);
    ASSERT_EQ(' ',                 canvas[8][0]);
    ASSERT_EQ(horizontal_beam,     canvas[9][0]);
    ASSERT_EQ(top_right_corner,    canvas[10][0]);

    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[10][1]);
    
    ASSERT_EQ(bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(horizontal_beam,     canvas[8][2]);
    ASSERT_EQ(horizontal_beam,     canvas[9][2]);
    ASSERT_EQ(bottom_right_corner, canvas[10][2]);
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, when_focussed_draws_a_highlighted_border)
{
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(terminalpp::element(top_left_corner, highlight_attribute),     canvas[0][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[1][0]);
    ASSERT_EQ(' ',                                                           canvas[2][0]);
    ASSERT_EQ('t',                                                           canvas[3][0]);
    ASSERT_EQ('i',                                                           canvas[4][0]);
    ASSERT_EQ('t',                                                           canvas[5][0]);
    ASSERT_EQ('l',                                                           canvas[6][0]);
    ASSERT_EQ('e',                                                           canvas[7][0]);
    ASSERT_EQ(' ',                                                           canvas[8][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[9][0]);
    ASSERT_EQ(terminalpp::element(top_right_corner, highlight_attribute),    canvas[10][0]);

    ASSERT_EQ(terminalpp::element(vertical_beam, highlight_attribute),       canvas[0][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam, highlight_attribute),       canvas[10][1]);
    
    ASSERT_EQ(terminalpp::element(bottom_left_corner, highlight_attribute),  canvas[0][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[1][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[2][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[3][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[4][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[5][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[6][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[7][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[8][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[9][2]);
    ASSERT_EQ(terminalpp::element(bottom_right_corner, highlight_attribute), canvas[10][2]);
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, redraws_when_associated_component_gains_focus)
{
    frame_.set_size({11, 3});
    
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
      terminalpp::rectangle{{0,  0}, {2,  1}},
      terminalpp::rectangle{{9,  0}, {2,  1}},
      terminalpp::rectangle{{0,  0}, {1,  3}},
      terminalpp::rectangle{{10, 0}, {1,  3}},
      terminalpp::rectangle{{0,  2}, {11, 1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, redraws_a_reduced_amount_when_associated_component_gains_focus_when_short)
{
    frame_.set_size({11, 2});
    
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
      terminalpp::rectangle{{0,  0}, {2,  1}},
      terminalpp::rectangle{{9,  0}, {2,  1}},
      terminalpp::rectangle{{0,  1}, {11, 1}},
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);

    for (auto region : redraw_regions)
    {
        ASSERT_GE(region.origin.x, 0);
        ASSERT_GE(region.origin.y, 0);
        ASSERT_GE(region.size.width, 0);
        ASSERT_GE(region.size.height, 0);
    }
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, redraws_a_reduced_amount_when_associated_component_gains_focus_when_only_title_is_showing)
{
    frame_.set_size({11, 1});
    
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
      terminalpp::rectangle{{0,  0}, {2,  1}},
      terminalpp::rectangle{{9,  0}, {2,  1}},
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);

    for (auto region : redraw_regions)
    {
        ASSERT_GE(region.origin.x, 0);
        ASSERT_GE(region.origin.y, 0);
        ASSERT_GE(region.size.width, 0);
        ASSERT_GE(region.size.height, 0);
    }
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, redraws_a_reduced_amount_when_associated_component_gains_focus_and_title_is_clipped)
{
    frame_.set_size({9, 3});
    
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
      terminalpp::rectangle{{0,  0}, {2,  1}},
      terminalpp::rectangle{{7,  0}, {2,  1}},
      terminalpp::rectangle{{0,  0}, {1,  3}},
      terminalpp::rectangle{{8,  0}, {1,  3}},
      terminalpp::rectangle{{0,  2}, {9,  1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(a_titled_frame_with_an_associated_unfocussed_component, redraws_a_reduced_amount_when_associated_component_gains_focus_and_frame_is_thin)
{
    frame_.set_size({4, 3});
    
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
      terminalpp::rectangle{{0,  0}, {4,  1}},
      terminalpp::rectangle{{0,  0}, {1,  3}},
      terminalpp::rectangle{{3,  0}, {1,  3}},
      terminalpp::rectangle{{0,  2}, {4,  1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(a_titled_frame_with_an_associated_focussed_component, when_unfocussed_draws_a_lowlit_border)
{
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(false));
    comp_->on_focus_lost();

    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(top_left_corner,     canvas[0][0]);
    ASSERT_EQ(horizontal_beam,     canvas[1][0]);
    ASSERT_EQ(' ',                 canvas[2][0]);
    ASSERT_EQ('t',                 canvas[3][0]);
    ASSERT_EQ('i',                 canvas[4][0]);
    ASSERT_EQ('t',                 canvas[5][0]);
    ASSERT_EQ('l',                 canvas[6][0]);
    ASSERT_EQ('e',                 canvas[7][0]);
    ASSERT_EQ(' ',                 canvas[8][0]);
    ASSERT_EQ(horizontal_beam,     canvas[9][0]);
    ASSERT_EQ(top_right_corner,    canvas[10][0]);

    ASSERT_EQ(vertical_beam,       canvas[0][1]);
    ASSERT_EQ(vertical_beam,       canvas[10][1]);
    
    ASSERT_EQ(bottom_left_corner,  canvas[0][2]);
    ASSERT_EQ(horizontal_beam,     canvas[1][2]);
    ASSERT_EQ(horizontal_beam,     canvas[2][2]);
    ASSERT_EQ(horizontal_beam,     canvas[3][2]);
    ASSERT_EQ(horizontal_beam,     canvas[4][2]);
    ASSERT_EQ(horizontal_beam,     canvas[5][2]);
    ASSERT_EQ(horizontal_beam,     canvas[6][2]);
    ASSERT_EQ(horizontal_beam,     canvas[7][2]);
    ASSERT_EQ(horizontal_beam,     canvas[8][2]);
    ASSERT_EQ(horizontal_beam,     canvas[9][2]);
    ASSERT_EQ(bottom_right_corner, canvas[10][2]);
}

TEST_F(a_titled_frame_with_an_associated_focussed_component, can_have_a_custom_highlight)
{
    terminalpp::attribute highlight_attribute = {
        terminalpp::ansi::graphics::colour::green,
        terminalpp::ansi::graphics::colour::magenta
    };
    
    auto const size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    frame_.set_highlight_attribute(highlight_attribute);

    ON_CALL(*comp_, do_has_focus())
        .WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas(size);
    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface, {{}, size});

    ASSERT_EQ(terminalpp::element(top_left_corner, highlight_attribute),     canvas[0][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[1][0]);
    ASSERT_EQ(' ',                                                           canvas[2][0]);
    ASSERT_EQ('t',                                                           canvas[3][0]);
    ASSERT_EQ('i',                                                           canvas[4][0]);
    ASSERT_EQ('t',                                                           canvas[5][0]);
    ASSERT_EQ('l',                                                           canvas[6][0]);
    ASSERT_EQ('e',                                                           canvas[7][0]);
    ASSERT_EQ(' ',                                                           canvas[8][0]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[9][0]);
    ASSERT_EQ(terminalpp::element(top_right_corner, highlight_attribute),    canvas[10][0]);

    ASSERT_EQ(terminalpp::element(vertical_beam, highlight_attribute),       canvas[0][1]);
    ASSERT_EQ(terminalpp::element(vertical_beam, highlight_attribute),       canvas[10][1]);
    
    ASSERT_EQ(terminalpp::element(bottom_left_corner, highlight_attribute),  canvas[0][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[1][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[2][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[3][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[4][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[5][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[6][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[7][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[8][2]);
    ASSERT_EQ(terminalpp::element(horizontal_beam, highlight_attribute),     canvas[9][2]);
    ASSERT_EQ(terminalpp::element(bottom_right_corner, highlight_attribute), canvas[10][2]);
}

TEST_F(a_titled_frame_with_an_associated_focussed_component, redraws_when_associated_component_gains_focus)
{
    frame_.set_size({11, 3});
    
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
      terminalpp::rectangle{{0,  0}, {2,  1}},
      terminalpp::rectangle{{9,  0}, {2,  1}},
      terminalpp::rectangle{{0,  0}, {1,  3}},
      terminalpp::rectangle{{10, 0}, {1,  3}},
      terminalpp::rectangle{{0,  2}, {11, 1}}
    };
    
    ASSERT_EQ(1, redraw_count);
    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
