#include "fill_canvas.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/element.hpp>
#include <terminalpp/virtual_key.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;
using testing::ValuesIn;

class a_new_edit : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_ = munin::make_edit();
};

TEST_F(a_new_edit, is_a_component)
{
    std::shared_ptr<munin::component> edit = edit_;
}

TEST_F(a_new_edit, has_a_1x1_preferred_size)
{
    // An edit control would always like a space for the cursor.
    ASSERT_EQ(terminalpp::extent(1, 1), edit_->get_preferred_size());
}

TEST_F(a_new_edit, has_its_cursor_enabled)
{
    ASSERT_TRUE(edit_->get_cursor_state());    
}

TEST_F(a_new_edit, has_its_cursor_at_home)
{
    ASSERT_EQ(terminalpp::point(0, 0), edit_->get_cursor_position());
}

TEST_F(a_new_edit, draws_blanks)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    edit_->set_position({1, 1});
    edit_->set_size({2, 1});
    
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    edit_->draw(surface, {{}, edit_->get_size()});
    
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, cvs[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][2]);
}

TEST_F(a_new_edit, inserting_text_changes_preferred_size_to_size_of_text)
{
    terminalpp::extent preferred_size;
    auto const callback = [&]{preferred_size = edit_->get_preferred_size();};
    edit_->on_preferred_size_changed.connect(callback);
    
    edit_->insert_text("test");
    
    // The expected preferred size is the length of the text and one extra
    // for the cursor.
    ASSERT_EQ(terminalpp::extent(5, 1), preferred_size);
}

TEST_F(a_new_edit, inserting_text_redraws_changed_text_area)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    edit_->set_position({1, 1});
    edit_->set_size({2, 1});
    
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    edit_->draw(surface, {{}, edit_->get_size()});

    
    edit_->on_redraw.connect(
        [&](auto const &regions)
        {
            for (auto const &region : regions)
            {
                edit_->draw(surface, region);
            }
        });
        
    edit_->insert_text("?!");

    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{'?'}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{'!'}, cvs[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][2]);
}

TEST_F(a_new_edit, draws_inserted_text_cursor_at_end)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    edit_->set_position({1, 1});
    edit_->set_size({2, 1});

    edit_->insert_text("za"_ts);
    
    ASSERT_EQ(terminalpp::point(1, 0), edit_->get_cursor_position());
    ASSERT_TRUE(edit_->get_cursor_state());
    
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    edit_->draw(surface, {{}, edit_->get_size()});

    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{'z'}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{'a'}, cvs[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][2]);
}

namespace {

using keypress_data = std::tuple<
    terminalpp::vk,      // character code
    terminalpp::element, // representation
    terminalpp::point    // expected cursor position
>;

class receiving_keypresses 
  : public testing::TestWithParam<keypress_data>
{
public:
    receiving_keypresses()
      : cvs_({4, 3}),
        surface_(cvs_)
    {
        fill_canvas(cvs_, 'x');
        surface_.offset_by({1, 1});
        
        edit_.on_redraw.connect(
            [this](auto const &regions)
            {
                for (auto const &region : regions)
                {
                    edit_.draw(surface_, region);
                }
            });
        
        edit_.set_position({1, 1});
        edit_.set_size({2, 1});
        
        edit_.on_redraw({
            {{0, 0}, edit_.get_size()}
        });
    }
    
protected:
    munin::edit          edit_;
    terminalpp::canvas   cvs_;
    munin::render_surface surface_;
};

}

TEST_P(receiving_keypresses, draws_appropriate_characters_and_moves_the_cursor)
{
    using std::get;
    
    auto const params = GetParam();
    auto const keypress = get<0>(params);
    auto const expected = get<1>(params);
    auto const cursor_pos = get<2>(params);
    
    edit_.event(terminalpp::virtual_key{keypress});
    
    ASSERT_EQ(cursor_pos, edit_.get_cursor_position());
    
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[0][1]);
    ASSERT_EQ(expected,                 cvs_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, cvs_[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs_[3][2]);
}

INSTANTIATE_TEST_SUITE_P(
    receiving_keypresses_test,
    receiving_keypresses,
    ValuesIn
    ({
        // Keypresses that represent printable characters get converted
        // to text and the cursor moves onward.
        keypress_data{ terminalpp::vk::uppercase_t, 'T', {1, 0} },
        keypress_data{ terminalpp::vk::lowercase_t, 't', {1, 0} },
        keypress_data{ terminalpp::vk::uppercase_z, 'Z', {1, 0} },
        keypress_data{ terminalpp::vk::space,       ' ', {1, 0} },
        keypress_data{ terminalpp::vk::dollar,      '$', {1, 0} },
        
        // Newline and control characters do not get converted (so the output
        // remains a space) and the cursor stays in the same location.
        keypress_data{ terminalpp::vk::lf,          ' ', {0, 0} },
        keypress_data{ terminalpp::vk::cr,          ' ', {0, 0} },
        keypress_data{ terminalpp::vk::nul,         ' ', {0, 0} },
        keypress_data{ terminalpp::vk::stx,         ' ', {0, 0} },

        // Specially, backspace or DEL do nothing on an empty edit
        keypress_data{ terminalpp::vk::bs,          ' ', {0, 0} },
        keypress_data{ terminalpp::vk::del,         ' ', {0, 0} },
        
        // Lastly, for an edit with no characters, the cursor can't be
        // moved with either cursor or other movement keys.
        keypress_data{ terminalpp::vk::cursor_up,    ' ', {0, 0} },
        keypress_data{ terminalpp::vk::cursor_down,  ' ', {0, 0} },
        keypress_data{ terminalpp::vk::cursor_left,  ' ', {0, 0} },
        keypress_data{ terminalpp::vk::cursor_right, ' ', {0, 0} },
        keypress_data{ terminalpp::vk::home,         ' ', {0, 0} },
        keypress_data{ terminalpp::vk::end,          ' ', {0, 0} },
        keypress_data{ terminalpp::vk::pgup,         ' ', {0, 0} },
        keypress_data{ terminalpp::vk::pgdn,         ' ', {0, 0} },
    })
);

namespace {
class an_edit_with_a_caret_out_of_view : public a_new_edit
{
public:
    an_edit_with_a_caret_out_of_view()
    {
        edit_->set_size({4, 1});
        edit_->insert_text("testtest"_ts);
        edit_->event(terminalpp::virtual_key{ terminalpp::vk::cursor_left});
        edit_->event(terminalpp::virtual_key{ terminalpp::vk::cursor_left});
        assert(edit_->get_cursor_position() == terminalpp::point(3, 0));
    }
};

}

TEST_F(an_edit_with_a_caret_out_of_view, updates_the_cursor_position_when_setting_the_edit_size)
{
    // Because the content is larger than the size and the caret is off
    // the end, the cursor should be moved to match as far right as it can.
    {
        edit_->set_size({6, 1});
        auto const expected_cursor_position = terminalpp::point{5, 0};
        ASSERT_EQ(expected_cursor_position, edit_->get_cursor_position());
    }

    // If we grow the size larger than the content, then the cursor should
    // max out where the caret was.
    {
        edit_->set_size({10, 1});
        auto const expected_cursor_position = terminalpp::point{6, 0};
        ASSERT_EQ(expected_cursor_position, edit_->get_cursor_position());
    }
}

namespace {

class an_edit_with_content : public a_new_edit
{
public:
    an_edit_with_content()
    {
        edit_->insert_text("test"_ts);
    }
};

}

TEST_F(an_edit_with_content, updates_the_preferred_size_when_text_is_deleted)
{
    auto preferred_size = terminalpp::extent{};
    edit_->on_preferred_size_changed.connect(
        [&]
        {
            preferred_size = edit_->get_preferred_size();
        });

    edit_->event(terminalpp::virtual_key{terminalpp::vk::bs});

    // Prefer enough space for "tes_" (including cursor).
    auto const expected_preferred_size = terminalpp::extent{4, 1};
    ASSERT_EQ(expected_preferred_size, preferred_size);
}
