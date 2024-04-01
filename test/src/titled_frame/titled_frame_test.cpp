#include "titled_frame_test.hpp"

#include "assert_similar.hpp"
#include "fill_canvas.hpp"
#include "redraw.hpp"
#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/render_surface.hpp>
#include <munin/titled_frame.hpp>
#include <terminalpp/attribute.hpp>
#include <terminalpp/string.hpp>

using namespace terminalpp::literals;  // NOLINT
using testing::Return;

namespace {
auto const top_left_corner = R"(+)"_ete.glyph_;
auto const top_right_corner = R"(+)"_ete.glyph_;
auto const bottom_left_corner = R"(+)"_ete.glyph_;
auto const bottom_right_corner = R"(+)"_ete.glyph_;
auto const horizontal_beam = R"(-)"_ete.glyph_;
auto const vertical_beam = R"(|)"_ete.glyph_;

auto const unicode_top_left_corner = R"(\U256D)"_ete.glyph_;
auto const unicode_top_right_corner = R"(\U256E)"_ete.glyph_;
auto const unicode_bottom_left_corner = R"(\U2570)"_ete.glyph_;
auto const unicode_bottom_right_corner = R"(\U256F)"_ete.glyph_;
auto const unicode_horizontal_beam = R"(\U2500)"_ete.glyph_;
auto const unicode_vertical_beam = R"(\U2502)"_ete.glyph_;

auto const highlight_attribute = terminalpp::attribute(
    terminalpp::graphics::colour::cyan,
    terminalpp::colour(),
    terminalpp::graphics::intensity::bold);

void assert_ansi_11x3_title_frame_with_attribute(
    terminalpp::canvas const &canvas,
    terminalpp::attribute const &attr = terminalpp::attribute{})
{
    assert_similar_canvas_block(
        {
  // clang-format off
          {
            { top_left_corner, attr },
            { horizontal_beam, attr },
            ' ',
            't',
            'i',
            't',
            'l',
            'e',
            ' ',
            { horizontal_beam, attr },
            { top_right_corner, attr },
          },
          {
            { vertical_beam, attr },
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            { vertical_beam, attr },
          },
          {
            { bottom_left_corner, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { horizontal_beam, attr },
            { bottom_right_corner, attr },
          },
  // clang-format on
    },
        canvas);
}

}  // namespace

TEST_F(a_titled_frame, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_titled_frame("title");
}

TEST_F(a_titled_frame_with_no_unicode_support, draws_a_border_with_title)
{
    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas);
}

TEST_F(a_titled_frame_with_no_unicode_support, left_justifies_text_when_grown)
{
    constexpr auto size = terminalpp::extent{13, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_similar_canvas_block(
        {
  // clang-format off
          {
            top_left_corner,
            horizontal_beam,
            ' ',
            't',
            'i',
            't',
            'l',
            'e',
            ' ',
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            top_right_corner,
          },
          {
            vertical_beam,
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            vertical_beam,
          },
          {
            bottom_left_corner,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            bottom_right_corner,
          },
  // clang-format on
    },
        canvas);
}

TEST_F(a_titled_frame_with_no_unicode_support, clips_the_title_when_shrunk)
{
    constexpr auto size = terminalpp::extent{9, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_similar_canvas_block(
        {
  // clang-format off
          {
            top_left_corner,
            horizontal_beam,
            ' ',
            'i',
            't',
            'l',
            ' ',
            horizontal_beam,
            top_right_corner,
          },
          {
            vertical_beam,
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            vertical_beam,
          },
          {
            bottom_left_corner,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            horizontal_beam,
            bottom_right_corner,
          },
  // clang-format on
    },
        canvas);
}

TEST_F(
    a_titled_frame_with_unicode_support, draws_a_border_with_box_drawing_glyphs)
{
    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_similar_canvas_block(
        {
  // clang-format off
          {
            unicode_top_left_corner,
            unicode_horizontal_beam,
            ' ',
            't',
            'i',
            't',
            'l',
            'e',
            ' ',
            unicode_horizontal_beam,
            unicode_top_right_corner,
          },
          {
            unicode_vertical_beam,
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            'X',
            unicode_vertical_beam,
          },
          {
            unicode_bottom_left_corner,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_horizontal_beam,
            unicode_bottom_right_corner,
          },
  // clang-format on
    },
        canvas);
}

TEST_F(a_titled_frame, can_be_displayed_with_a_custom_lowlight)
{
    static constexpr auto lowlight_attribute = terminalpp::attribute(
        terminalpp::graphics::colour::green,
        terminalpp::colour(),
        terminalpp::graphics::intensity::bold);

    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    frame_.set_lowlight_attribute(lowlight_attribute);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas, lowlight_attribute);
}

class a_titled_frame_with_an_associated_unfocussed_component
  : public a_titled_frame_with_no_unicode_support
{
protected:
    a_titled_frame_with_an_associated_unfocussed_component()
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
    }

    std::shared_ptr<mock_component> comp_ = make_mock_component();
};

class a_titled_frame_with_an_associated_focussed_component
  : public a_titled_frame_with_no_unicode_support
{
protected:
    a_titled_frame_with_an_associated_focussed_component()
    {
        frame_.highlight_on_focus(comp_);
        ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
        comp_->on_focus_set();
    }

    std::shared_ptr<mock_component> comp_ = make_mock_component();
};

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    draws_a_lowlighted_border)
{
    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas);
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    when_focussed_draws_a_highlighted_border)
{
    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas, highlight_attribute);
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    redraws_when_associated_component_gains_focus)
{
    frame_.set_size({11, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0},  {2, 1} },
        terminalpp::rectangle{{9, 0},  {2, 1} },
        terminalpp::rectangle{{0, 0},  {1, 3} },
        terminalpp::rectangle{{10, 0}, {1, 3} },
        terminalpp::rectangle{{0, 2},  {11, 1}}
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    redraws_a_reduced_amount_when_associated_component_gains_focus_when_short)
{
    frame_.set_size({11, 2});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0}, {2, 1} },
        terminalpp::rectangle{{9, 0}, {2, 1} },
        terminalpp::rectangle{{0, 1}, {11, 1}},
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);

    for (auto region : redraw_regions)
    {
        ASSERT_GE(region.origin_.x_, 0);
        ASSERT_GE(region.origin_.y_, 0);
        ASSERT_GE(region.size_.width_, 0);
        ASSERT_GE(region.size_.height_, 0);
    }
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    redraws_a_reduced_amount_when_associated_component_gains_focus_when_only_title_is_showing)
{
    frame_.set_size({11, 1});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0}, {2, 1}},
        terminalpp::rectangle{{9, 0}, {2, 1}},
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);

    for (auto region : redraw_regions)
    {
        ASSERT_GE(region.origin_.x_, 0);
        ASSERT_GE(region.origin_.y_, 0);
        ASSERT_GE(region.size_.width_, 0);
        ASSERT_GE(region.size_.height_, 0);
    }
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    redraws_a_reduced_amount_when_associated_component_gains_focus_and_title_is_clipped)
{
    frame_.set_size({9, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0}, {2, 1}},
        terminalpp::rectangle{{7, 0}, {2, 1}},
        terminalpp::rectangle{{0, 0}, {1, 3}},
        terminalpp::rectangle{{8, 0}, {1, 3}},
        terminalpp::rectangle{{0, 2}, {9, 1}}
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(
    a_titled_frame_with_an_associated_unfocussed_component,
    redraws_a_reduced_amount_when_associated_component_gains_focus_and_frame_is_thin)
{
    frame_.set_size({4, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0}, {4, 1}},
        terminalpp::rectangle{{0, 0}, {1, 3}},
        terminalpp::rectangle{{3, 0}, {1, 3}},
        terminalpp::rectangle{{0, 2}, {4, 1}}
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}

TEST_F(
    a_titled_frame_with_an_associated_focussed_component,
    when_unfocussed_draws_a_lowlit_border)
{
    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
    comp_->on_focus_lost();

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas);
}

TEST_F(
    a_titled_frame_with_an_associated_focussed_component,
    can_have_a_custom_highlight)
{
    terminalpp::attribute highlight_attribute = {
        terminalpp::graphics::colour::green,
        terminalpp::graphics::colour::magenta};

    constexpr auto size = terminalpp::extent{11, 3};
    frame_.set_size(size);
    frame_.set_highlight_attribute(highlight_attribute);

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(true));
    comp_->on_focus_set();

    terminalpp::canvas canvas(size);
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas, surface_capabilities_};
    frame_.draw(surface);

    assert_ansi_11x3_title_frame_with_attribute(canvas, highlight_attribute);
}

TEST_F(
    a_titled_frame_with_an_associated_focussed_component,
    redraws_when_associated_component_gains_focus)
{
    frame_.set_size({11, 3});

    std::vector<terminalpp::rectangle> redraw_regions;
    frame_.on_redraw.connect(append_regions_to_container(redraw_regions));

    ON_CALL(*comp_, do_has_focus()).WillByDefault(Return(false));
    comp_->on_focus_lost();

    std::vector<terminalpp::rectangle> const expected_redraw_regions = {
        terminalpp::rectangle{{0, 0},  {2, 1} },
        terminalpp::rectangle{{9, 0},  {2, 1} },
        terminalpp::rectangle{{0, 0},  {1, 3} },
        terminalpp::rectangle{{10, 0}, {1, 3} },
        terminalpp::rectangle{{0, 2},  {11, 1}}
    };

    assert_equivalent_redraw_regions(expected_redraw_regions, redraw_regions);
}
