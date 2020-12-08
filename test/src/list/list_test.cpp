#include <munin/list.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/virtual_key.hpp>
#include <boost/range/algorithm_ext/insert.hpp>
#include <gtest/gtest.h>

using namespace terminalpp::literals;

TEST(a_list, is_a_component)
{
    std::shared_ptr<munin::list> list = munin::make_list();
    std::shared_ptr<munin::component> component = list;
}

namespace {

class a_new_list : public testing::Test
{
protected:
    a_new_list()
    {
        list_->set_size(list_size);

        terminalpp::for_each_in_region(
            canvas_,
            {{}, canvas_.size()},
            [](terminalpp::element &elem,
            terminalpp::coordinate_type column,
            terminalpp::coordinate_type row)
            {
                elem = 'X';
            });
    }

    static constexpr terminalpp::extent list_size{6, 3};

    std::shared_ptr<munin::list> list_ { munin::make_list() };
    terminalpp::canvas canvas_{list_size};
    munin::render_surface surface_{canvas_};
};

constexpr terminalpp::extent a_new_list::list_size;

}

TEST_F(a_new_list, has_a_zero_preferred_size)
{
    constexpr auto expected = terminalpp::extent{0, 0};
    ASSERT_EQ(expected, list_->get_preferred_size());
}

TEST_F(a_new_list, has_no_selected_item)
{
    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_new_list, draws_empty_space)
{
    list_->draw(surface_, {{}, list_size});

    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_new_list, requests_a_redraw_when_setting_the_list_items)
{
    std::vector<terminalpp::rectangle> requested_regions;
    list_->on_redraw.connect(
        [&requested_regions](auto const &regions)
        {
            requested_regions = regions;
        });
        
    std::vector<terminalpp::string> items = {"test"_ts};
    list_->set_items(items);

    auto const expected_bounds = terminalpp::rectangle{{}, list_size};
    ASSERT_EQ(1u, requested_regions.size());
    ASSERT_EQ(expected_bounds, requested_regions[0]);
}

TEST_F(a_new_list, signals_a_preferred_size_change_when_setting_the_list_items)
{
    bool preferred_size_changed = false;
    list_->on_preferred_size_changed.connect(
        [&preferred_size_changed]()
        {
            preferred_size_changed = true;
        });
        
    std::vector<terminalpp::string> items = {"test"_ts};
    list_->set_items(items);

    ASSERT_TRUE(preferred_size_changed);    
}

TEST_F(a_new_list, signals_a_cursor_position_change_when_setting_the_list_items)
{
    bool cursor_position_changed = false;
    list_->on_cursor_position_changed.connect(
        [&cursor_position_changed]()
        {
            cursor_position_changed = true;
        });

    std::vector<terminalpp::string> items = {"test"_ts};
    list_->set_items(items);
    ASSERT_TRUE(cursor_position_changed);
}


TEST_F(a_new_list, can_be_clicked)
{
    list_->event(
        terminalpp::ansi::mouse::report{
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            1,
            1
        });

    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_new_list, ignores_the_up_key)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});
    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_new_list, ignores_the_down_key)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});
    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_new_list, has_a_zero_cursor_position)
{
    ASSERT_EQ(terminalpp::point(0, 0), list_->get_cursor_position());
}

TEST_F(a_new_list, has_a_disabled_cursor)
{
    ASSERT_FALSE(list_->get_cursor_state());
}

namespace {

class a_list_with_an_item : public a_new_list
{
protected:
    a_list_with_an_item()
    {
        list_->set_items({item_text});
    }

    terminalpp::string const item_text = "test"_ts;
};

}

TEST_F(a_list_with_an_item, has_no_selected_item)
{
    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_list_with_an_item, has_a_preferred_size_of_that_item)
{
    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(item_text.size()),
        1
    };

    ASSERT_EQ(expected_preferred_size, list_->get_preferred_size());
}

TEST_F(a_list_with_an_item, draws_that_item)
{
    list_->draw(surface_, {{}, list_size});

    ASSERT_EQ(terminalpp::element{'t'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{'s'}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_an_item, reports_a_selected_item_as_selected)
{
    list_->select_item(0);
    auto const selected_item_index = list_->get_selected_item_index();

    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_an_item, sends_item_changed_signal_when_an_item_is_selected)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->select_item(0);
    ASSERT_TRUE(item_changed);
}

TEST_F(a_list_with_an_item, signals_a_changed_cursor_position_when_an_item_is_selected)
{
    bool cursor_position_changed = false;
    list_->on_cursor_position_changed.connect(
        [&cursor_position_changed]()
        {
            cursor_position_changed = true;
        });

    list_->select_item(0);
    ASSERT_TRUE(cursor_position_changed);
}

TEST_F(a_list_with_an_item, selects_the_item_when_it_is_clicked)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->event(
        terminalpp::ansi::mouse::report {
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            0,
            0
        });

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_an_item, deselects_the_item_when_empty_space_is_clicked)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->event(
        terminalpp::ansi::mouse::report {
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            0,
            1
        });

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_list_with_an_item, selects_the_item_when_the_up_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });

    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_an_item, selects_the_item_when_the_down_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

namespace {

class a_list_with_a_selected_item : public a_list_with_an_item
{
protected:
    a_list_with_a_selected_item()
    {
        list_->select_item(0);
    }
};

}

TEST_F(a_list_with_a_selected_item, draws_that_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element('t', negative_attr), canvas_[0][0]);
    ASSERT_EQ(terminalpp::element('e', negative_attr), canvas_[1][0]);
    ASSERT_EQ(terminalpp::element('s', negative_attr), canvas_[2][0]);
    ASSERT_EQ(terminalpp::element('t', negative_attr), canvas_[3][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[4][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[5][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[0][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[1][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[2][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[3][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[4][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[5][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[0][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[1][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[2][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[3][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[4][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas_[5][2]);
}

TEST_F(a_list_with_a_selected_item, deselects_the_item_when_the_up_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_list_with_a_selected_item, deselects_the_item_when_the_down_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

namespace {

class a_list_with_two_items : public a_new_list
{
protected:
    a_list_with_two_items()
    {
        list_->set_items({item_text0, item_text1});
    }

    terminalpp::string const item_text0 = "l0"_ts;
    terminalpp::string const item_text1 = "line1"_ts;
};

}

TEST_F(a_list_with_two_items, has_a_preferred_size_of_the_largest_item_and_the_number_of_items)
{
    auto const expected_preferred_size = terminalpp::extent{
        static_cast<terminalpp::coordinate_type>(item_text1.size()),
        2
    };

    ASSERT_EQ(expected_preferred_size, list_->get_preferred_size());
}

TEST_F(a_list_with_two_items, draws_those_items)
{
    list_->draw(surface_, {{}, list_size});

    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'0'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'i'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{'n'}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{'1'}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items, reports_a_selected_item_as_selected)
{
    list_->select_item(0);
    auto selected_item_index = list_->get_selected_item_index();

    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);

    list_->select_item(1);
    selected_item_index = list_->get_selected_item_index();

    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(1, *selected_item_index);
}

TEST_F(a_list_with_two_items, sends_item_changed_signal_when_an_item_is_selected)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->select_item(0);
    ASSERT_TRUE(item_changed);

    item_changed = false;
    list_->select_item(1);
    ASSERT_TRUE(item_changed);
}

TEST_F(a_list_with_two_items, selects_the_first_item_when_it_is_clicked)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->event(
        terminalpp::ansi::mouse::report {
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            0,
            0
        });

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_two_items, selects_the_second_item_when_it_is_clicked)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]()
        {
            item_changed = true;
        });

    list_->event(
        terminalpp::ansi::mouse::report {
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            0,
            1
        });

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(1, *selected_item_index);
}

TEST_F(a_list_with_two_items, selects_the_second_item_when_the_up_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(1, *selected_item_index);
}

TEST_F(a_list_with_two_items, selects_the_first_item_when_the_down_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_an_item, selects_no_item_when_empty_space_is_clicked)
{
    list_->event(
        terminalpp::ansi::mouse::report {
            terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
            0,
            2
        });

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

namespace {

class a_list_with_two_items_and_the_first_selected : public a_list_with_two_items
{
protected:
    a_list_with_two_items_and_the_first_selected()
    {
        list_->select_item(0);
    }
};

}

TEST_F(a_list_with_two_items_and_the_first_selected, draws_the_first_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element('l', negative_attr), canvas_[0][0]);
    ASSERT_EQ(terminalpp::element('0', negative_attr), canvas_[1][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[2][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[3][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[4][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'i'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{'n'}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{'1'}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items_and_the_first_selected, redraws_the_items_when_the_item_is_deselected)
{
    list_->draw(surface_, {{}, list_size});

    std::vector<terminalpp::rectangle> redraw_regions;
    list_->on_redraw.connect(
        [&redraw_regions](auto const &regions)
        {
            boost::insert(redraw_regions, redraw_regions.end(), regions);
        });

    list_->select_item(boost::none);

    for (auto const &region : redraw_regions)
    {
        list_->draw(surface_, region);
    }

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'0'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'i'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{'n'}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{'1'}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items_and_the_first_selected, selects_the_second_item_when_the_down_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(1, *selected_item_index);
}

TEST_F(a_list_with_two_items_and_the_first_selected, deselects_the_first_item_when_the_up_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_list_with_two_items_and_the_first_selected, has_a_cursor_position_on_the_home_row)
{
    ASSERT_EQ(terminalpp::point(0, 0), list_->get_cursor_position());
}

namespace {

class a_list_with_two_items_and_the_second_selected : public a_list_with_two_items
{
protected:
    a_list_with_two_items_and_the_second_selected()
    {
        list_->select_item(1);
    }
};

}

TEST_F(a_list_with_two_items_and_the_second_selected, draws_the_second_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'0'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element('l', negative_attr), canvas_[0][1]);
    ASSERT_EQ(terminalpp::element('i', negative_attr), canvas_[1][1]);
    ASSERT_EQ(terminalpp::element('n', negative_attr), canvas_[2][1]);
    ASSERT_EQ(terminalpp::element('e', negative_attr), canvas_[3][1]);
    ASSERT_EQ(terminalpp::element('1', negative_attr), canvas_[4][1]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items_and_the_second_selected, deselects_the_second_item_when_the_down_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
}

TEST_F(a_list_with_two_items_and_the_second_selected, selects_the_first_item_when_the_up_key_is_pressed)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });
        
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_two_items_and_the_first_selected, redraws_the_items_when_the_second_is_selected)
{
    list_->draw(surface_, {{}, list_size});

    std::vector<terminalpp::rectangle> redraw_regions;
    list_->on_redraw.connect(
        [&redraw_regions](auto const &regions)
        {
            boost::insert(redraw_regions, redraw_regions.end(), regions);
        });

    list_->select_item(1);

    for (auto const &region : redraw_regions)
    {
        list_->draw(surface_, region);
    }

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'0'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element('l', negative_attr), canvas_[0][1]);
    ASSERT_EQ(terminalpp::element('i', negative_attr), canvas_[1][1]);
    ASSERT_EQ(terminalpp::element('n', negative_attr), canvas_[2][1]);
    ASSERT_EQ(terminalpp::element('e', negative_attr), canvas_[3][1]);
    ASSERT_EQ(terminalpp::element('1', negative_attr), canvas_[4][1]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items_and_the_second_selected, redraws_the_items_when_the_first_is_selected)
{
    list_->draw(surface_, {{}, list_size});

    std::vector<terminalpp::rectangle> redraw_regions;
    list_->on_redraw.connect(
        [&redraw_regions](auto const &regions)
        {
            boost::insert(redraw_regions, redraw_regions.end(), regions);
        });

    list_->select_item(0);

    for (auto const &region : redraw_regions)
    {
        list_->draw(surface_, region);
    }

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element('l', negative_attr), canvas_[0][0]);
    ASSERT_EQ(terminalpp::element('0', negative_attr), canvas_[1][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[2][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[3][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[4][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'i'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{'n'}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{'1'}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_two_items_and_the_second_selected, redraws_the_items_when_the_item_is_deselected)
{
    list_->draw(surface_, {{}, list_size});

    std::vector<terminalpp::rectangle> redraw_regions;
    list_->on_redraw.connect(
        [&redraw_regions](auto const &regions)
        {
            boost::insert(redraw_regions, redraw_regions.end(), regions);
        });

    list_->select_item(boost::none);

    for (auto const &region : redraw_regions)
    {
        list_->draw(surface_, region);
    }

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][0]);
    ASSERT_EQ(terminalpp::element{'0'}, canvas_[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][0]);
    ASSERT_EQ(terminalpp::element{'l'}, canvas_[0][1]);
    ASSERT_EQ(terminalpp::element{'i'}, canvas_[1][1]);
    ASSERT_EQ(terminalpp::element{'n'}, canvas_[2][1]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas_[3][1]);
    ASSERT_EQ(terminalpp::element{'1'}, canvas_[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas_[5][2]);
}

TEST_F(a_list_with_a_selected_item, when_items_are_set_to_empty_list_deselects_item)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });

    list_->set_items({});

    ASSERT_TRUE(item_changed);
    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_list_with_two_items_and_the_first_selected, when_items_are_set_to_empty_list_deselects_item)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });

    list_->set_items({});

    ASSERT_TRUE(item_changed);
    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_list_with_two_items_and_the_second_selected, when_items_are_set_to_empty_list_deselects_item)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });

    list_->set_items({});

    ASSERT_TRUE(item_changed);
    ASSERT_FALSE(list_->get_selected_item_index().is_initialized());
}

TEST_F(a_list_with_a_selected_item, when_items_are_set_to_a_one_element_list_the_first_item_remains_selected)
{
    list_->set_items({"new item"_ts});

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_two_items_and_the_second_selected, when_items_are_set_to_a_one_element_list_the_first_item_becomes_selected)
{
    bool item_changed = false;
    list_->on_item_changed.connect(
        [&item_changed]
        {
            item_changed = true;
        });

    list_->set_items({"new item"_ts});

    ASSERT_TRUE(item_changed);

    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_TRUE(selected_item_index.is_initialized());
    ASSERT_EQ(0, *selected_item_index);
}

TEST_F(a_list_with_two_items_and_the_second_selected, has_a_cursor_position_on_the_second_row)
{
    ASSERT_EQ(terminalpp::point(0, 1), list_->get_cursor_position());
}
