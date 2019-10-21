#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/element.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;

class a_new_edit : public testing::Test
{
protected:
    void fill_canvas(
        terminalpp::canvas &canvas, 
        terminalpp::element const &fill = ' ')
    {
        terminalpp::for_each_in_region(
            canvas,
            {{}, canvas.size()},
            [fill](terminalpp::element &elem,
                   terminalpp::coordinate_type column,
                   terminalpp::coordinate_type row)
            {
                elem = fill;
            });
    }

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

TEST_F(a_new_edit, draws_inserted_text_cursor_at_end)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    edit_->set_position({1, 1});
    edit_->set_size({2, 1});

    edit_->insert_text("za"_ts);
    
    ASSERT_EQ(terminalpp::point(2, 0), edit_->get_cursor_position());
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
