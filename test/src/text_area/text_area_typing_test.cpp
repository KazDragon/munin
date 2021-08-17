#include "text_area_test.hpp"
#include <terminalpp/virtual_key.hpp>

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

