#include "assert_similar.hpp"
#include "fill_canvas.hpp"

#include <gtest/gtest.h>
#include <munin/image.hpp>
#include <munin/render_surface.hpp>

using namespace terminalpp::literals;  // NOLINT

TEST(a_new_image, has_a_zero_preferred_size)
{
    munin::image image;
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(a_new_image_with_a_fill, has_a_zero_preferred_size)
{
    munin::image image(terminalpp::element('X'));
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(a_new_image_with_single_line_empty_content, has_a_zero_preferred_size)
{
    munin::image image(""_ts);
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(
    a_new_image_with_single_line_content,
    has_a_preferred_size_with_the_width_of_that_line)
{
    auto const content = "abcde"_ts;
    munin::image image(content);

    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(content.size()), 1};

    ASSERT_EQ(expected_preferred_size, image.get_preferred_size());
}

TEST(
    a_new_image_with_single_line_content_and_a_fill,
    has_a_preferred_size_with_the_width_of_that_line)
{
    auto const content = "abcde"_ts;
    munin::image image(content, terminalpp::element('Y'));

    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(content.size()), 1};

    ASSERT_EQ(expected_preferred_size, image.get_preferred_size());
}

TEST(a_new_image_with_empty_multi_line_content, has_a_zero_preferred_size)
{
    munin::image image(std::vector<terminalpp::string>{});
    ASSERT_EQ(terminalpp::extent(0, 0), image.get_preferred_size());
}

TEST(
    a_new_image_with_multi_line_content,
    has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
    auto const content =
        std::vector<terminalpp::string>{"abcde"_ts, "abcdefg"_ts};

    munin::image image(content);

    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(content[1].size()), 2};

    ASSERT_EQ(expected_preferred_size, image.get_preferred_size());
}

TEST(
    a_new_image_with_multi_line_content_and_a_fill,
    has_a_preferred_size_with_the_width_of_the_longest_line_and_height_of_the_number_of_lines)
{
    auto const content =
        std::vector<terminalpp::string>{"abcde"_ts, "abcdefg"_ts};

    munin::image image(content, 'Z');

    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(content[1].size()), 2};

    ASSERT_EQ(expected_preferred_size, image.get_preferred_size());
}

TEST(a_new_image, draws_whitespace_on_the_canvas)
{
    munin::image image;
    image.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "  X"_ts,
          "  X"_ts,
          "XXX"_ts,
            // clang-format on
        },
        canvas);
}

TEST(a_new_image, refuses_focus)
{
    munin::image image;
    image.set_focus();
    ASSERT_FALSE(image.has_focus());
}

TEST(a_new_image_that_can_receive_focus, receives_focus)
{
    munin::image image;
    image.set_can_receive_focus(true);
    image.set_focus();
    ASSERT_TRUE(image.has_focus());
}

TEST(a_new_image_with_a_fill, draws_fill_on_the_canvas)
{
    munin::image image('Z');
    image.set_size({2, 2});

    terminalpp::canvas canvas({3, 3});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "ZZX"_ts,
          "ZZX"_ts,
          "XXX"_ts,
            // clang-format on
        },
        canvas);
}

TEST(
    a_new_image_with_a_single_line_content,
    draws_that_content_centred_with_whitespace_background)
{
    munin::image image("abc"_ts);
    image.set_size({5, 3});

    terminalpp::canvas canvas({5, 3});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "     "_ts,
          " abc "_ts,
          "     "_ts,
            // clang-format on
        },
        canvas);
}

TEST(
    a_new_image_with_a_single_line_content_and_fill,
    draws_that_content_centred_with_filled_background)
{
    munin::image image("abc"_ts, 'Z');
    image.set_size({5, 3});

    terminalpp::canvas canvas({5, 3});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "ZZZZZ"_ts,
          "ZabcZ"_ts,
          "ZZZZZ"_ts,
            // clang-format on
        },
        canvas);
}

TEST(
    a_new_image_with_multi_line_content,
    draws_that_content_centred_with_whitespace_background)
{
    munin::image image({"ab"_ts, "d"_ts, "efg"_ts});
    image.set_size({5, 5});

    terminalpp::canvas canvas({5, 5});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "     "_ts,
          " ab  "_ts,
          " d   "_ts,
          " efg "_ts,
          "     "_ts,
            // clang-format on
        },
        canvas);
}

TEST(
    a_new_image_with_multi_line_content_with_fill,
    draws_that_content_centred_with_fill_background)
{
    munin::image image({"ab"_ts, "d"_ts, "efg"_ts}, 'T');
    image.set_size({5, 5});

    terminalpp::canvas canvas({5, 5});
    fill_canvas(canvas, 'X');

    munin::render_surface surface{canvas};
    image.draw(surface, {{}, image.get_size()});

    assert_similar_canvas_block(
        {
            // clang-format off
          "TTTTT"_ts,
          "TabTT"_ts,
          "TdTTT"_ts,
          "TefgT"_ts,
          "TTTTT"_ts,
            // clang-format on
        },
        canvas);
}

TEST(make_image_with_no_content, makes_a_new_default_image)
{
    std::shared_ptr<munin::image> image = munin::make_image();
    ASSERT_EQ(image->to_json(), munin::image{}.to_json());
}

TEST(make_image_with_no_content_and_fill, makes_a_new_default_image_with_fill)
{
    auto const fill = terminalpp::element('x');
    auto image = munin::make_image(fill);

    ASSERT_EQ(image->to_json(), munin::image(fill).to_json());
}

TEST(make_image_with_a_string_content, makes_a_single_line_image)
{
    auto const content = "test"_ts;
    std::shared_ptr<munin::image> image = munin::make_image(content);

    ASSERT_EQ(image->to_json(), munin::image(content).to_json());
}

TEST(
    make_image_with_a_string_content_and_fill,
    makes_a_single_line_image_with_fill)
{
    auto const content = "test"_ts;
    auto const fill = terminalpp::element('z');
    std::shared_ptr<munin::image> image = munin::make_image(content, fill);

    ASSERT_EQ(image->to_json(), munin::image(content, fill).to_json());
}

TEST(make_image_with_a_vector_content, makes_a_multi_line_image)
{
    std::vector<terminalpp::string> const content = {"ab", "cd"};
    std::shared_ptr<munin::image> image = munin::make_image(content);
    ASSERT_EQ(image->to_json(), munin::image(content).to_json());
}

TEST(
    make_image_with_a_vector_content_and_fill,
    makes_a_multi_line_image_with_fill)
{
    std::vector<terminalpp::string> const content = {"ab", "cd"};
    auto const fill = terminalpp::element('Q');
    std::shared_ptr<munin::image> image = munin::make_image(content, fill);
    ASSERT_EQ(image->to_json(), munin::image(content, fill).to_json());
}
