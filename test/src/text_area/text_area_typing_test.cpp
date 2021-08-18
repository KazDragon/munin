#include "text_area_test.hpp"
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/virtual_key.hpp>

using namespace terminalpp::literals;

TEST_F(a_text_area, appends_typed_keys_at_the_cursor)
{
    {
        auto const keypress_a = terminalpp::virtual_key {
            terminalpp::vk::lowercase_a,
            terminalpp::vk_modifier::none,
            1
        };

        text_area_.event(keypress_a);

        auto const expected_text = terminalpp::string("a");

        ASSERT_EQ(expected_text, text_area_.get_text());
    }

    {
        auto const keypress_b = terminalpp::virtual_key {
            terminalpp::vk::uppercase_b,
            terminalpp::vk_modifier::none,
            1,
        };

        text_area_.event(keypress_b);

        auto const expected_text = terminalpp::string("aB");

        ASSERT_EQ(expected_text, text_area_.get_text());
    }

    {
        auto const keypress_c = terminalpp::virtual_key {
            terminalpp::vk::uppercase_c,
            terminalpp::vk_modifier::none,
            1,
        };

        text_area_.set_caret_position(0);
        text_area_.event(keypress_c);

        auto const expected_text = terminalpp::string("CaB");

        ASSERT_EQ(expected_text, text_area_.get_text());
    }
}

TEST_F(a_text_area, requests_a_redraw_when_text_is_added_via_keypress)
{
    terminalpp::canvas cvs({3, 3});
    munin::render_surface surface{cvs};

    text_area_.on_redraw.connect(
        [&surface, this](auto const &regions)
        {
            for (auto const &region : regions)
            {
                text_area_.draw(surface, region);
            }
        });

    text_area_.set_size({3, 3});

    auto const keypress_d = terminalpp::virtual_key {
        terminalpp::vk::lowercase_d,
        terminalpp::vk_modifier::none,
        1
    };

    text_area_.event(keypress_d);

    ASSERT_EQ(terminalpp::element{'d'}, cvs[0][0]);
}

TEST_F(a_text_area, updates_the_cursor_position_when_text_is_added_via_keypress)
{
    auto cursor_position = terminalpp::point{};
    text_area_.on_cursor_position_changed.connect(
        [&cursor_position, this]()
        {
            cursor_position = text_area_.get_cursor_position();
        });

    text_area_.set_size({3, 3});

    auto const keypress_e = terminalpp::virtual_key {
        terminalpp::vk::uppercase_e,
        terminalpp::vk_modifier::none,
        1
    };

    text_area_.event(keypress_e);

    auto const expected_cursor_position = terminalpp::point{1, 0};
    ASSERT_EQ(expected_cursor_position, cursor_position);
}

TEST_F(a_text_area, deletes_text_before_the_cursor_when_backspace_is_pressed)
{
    auto cvs = terminalpp::canvas({10, 3});
    text_area_.set_size({10, 3});

    text_area_.on_redraw.connect(
        [&cvs, this](auto const &regions)
        {
            for (auto const &region : regions)
            {
                munin::render_surface surface{cvs};
                text_area_.draw(surface, region);
            }
        });

    auto cursor_position = terminalpp::point{};
    text_area_.on_cursor_position_changed.connect(
        [&cursor_position, this]()
        {
            cursor_position = text_area_.get_cursor_position();
        });

    text_area_.insert_text("test"_ts);
    text_area_.set_caret_position(3);

    auto const keypress_bs = terminalpp::virtual_key {
        terminalpp::vk::bs,
        terminalpp::vk_modifier::none,
        1
    };

    text_area_.event(keypress_bs);

    EXPECT_EQ("tes"_ts, text_area_.get_text());
    EXPECT_EQ(terminalpp::point(2, 0), cursor_position);
    EXPECT_EQ(terminalpp::element('t'), cvs[0][0]);
    EXPECT_EQ(terminalpp::element('e'), cvs[1][0]);
    EXPECT_EQ(terminalpp::element('s'), cvs[2][0]);
    EXPECT_EQ(terminalpp::element(' '), cvs[3][0]);
}

TEST_F(a_text_area, deletes_text_before_the_cursor_when_del_is_pressed)
{
    auto cvs = terminalpp::canvas({10, 3});
    text_area_.set_size({10, 3});

    text_area_.on_redraw.connect(
        [&cvs, this](auto const &regions)
        {
            for (auto const &region : regions)
            {
                munin::render_surface surface{cvs};
                text_area_.draw(surface, region);
            }
        });

    auto cursor_position = terminalpp::point{};
    text_area_.on_cursor_position_changed.connect(
        [&cursor_position, this]()
        {
            cursor_position = text_area_.get_cursor_position();
        });

    text_area_.insert_text("test"_ts);
    text_area_.set_caret_position(3);

    auto const keypress_del = terminalpp::virtual_key {
        terminalpp::vk::del,
        terminalpp::vk_modifier::none,
        1
    };

    text_area_.event(keypress_del);

    EXPECT_EQ("tes"_ts, text_area_.get_text());
    EXPECT_EQ(terminalpp::point(2, 0), cursor_position);
    EXPECT_EQ(terminalpp::element('t'), cvs[0][0]);
    EXPECT_EQ(terminalpp::element('e'), cvs[1][0]);
    EXPECT_EQ(terminalpp::element('s'), cvs[2][0]);
    EXPECT_EQ(terminalpp::element(' '), cvs[3][0]);
}
