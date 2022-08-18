#include "similar_canvas.hpp"
#include <munin/list.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
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

        list_->on_redraw.connect([this](auto const &regions) {
            for(auto const &region : regions)
            {
                list_->draw(surface_, region);
            }
        });

        list_->on_preferred_size_changed.connect([this] {
            preferred_size_changed_ = true;
        });

        list_->on_cursor_position_changed.connect([this] {
            cursor_position_changed_ = true;
        });

        list_->on_item_changed.connect([this] {
            item_changed_ = true;
            selected_item_index_ = list_->get_selected_item_index();
        });
    }

    void reset()
    {
        preferred_size_changed_ = false;
        cursor_position_changed_ = false;
        item_changed_ = false;
    }

    static constexpr terminalpp::extent list_size{6, 3};

    std::shared_ptr<munin::list> list_ { munin::make_list() };
    terminalpp::canvas canvas_{list_size};
    munin::render_surface surface_{canvas_};

    bool preferred_size_changed_ = false;
    bool cursor_position_changed_ = false;
    bool item_changed_ = false;
    std::optional<int> selected_item_index_ { list_->get_selected_item_index() };
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
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_new_list, draws_empty_space)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "      "_ts,
          "      "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_new_list, redraws_when_setting_the_list_items)
{
    list_->set_items({"test"_ts});

    assert_similar_canvas(
        { "test  "_ts,
          "      "_ts,
          "      "_ts, },
        canvas_);
}

TEST_F(a_new_list, signals_a_preferred_size_change_when_setting_the_list_items)
{
    list_->set_items({"test"_ts});
    ASSERT_TRUE(preferred_size_changed_);
}

TEST_F(a_new_list, signals_a_cursor_position_change_when_setting_the_list_items)
{
    list_->set_items({"test"_ts});
    ASSERT_TRUE(cursor_position_changed_);
}

TEST_F(a_new_list, can_be_clicked)
{
    list_->event(
        terminalpp::mouse::event{
            terminalpp::mouse::event_type::left_button_down,
            { 1, 1 }
        });

    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_new_list, ignores_the_up_key)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_new_list, ignores_the_down_key)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});
    ASSERT_FALSE(selected_item_index_.has_value());
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
        reset();
    }

    terminalpp::string const item_text = "test"_ts;
};

}

TEST_F(a_list_with_an_item, has_no_selected_item)
{
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_an_item, has_a_preferred_size_of_that_item)
{
    auto const expected_preferred_size = terminalpp::extent{
        terminalpp::coordinate_type(item_text.size()),
        1
    };

    ASSERT_EQ(expected_preferred_size, list_->get_preferred_size());
}

TEST_F(a_list_with_an_item, draws_that_item)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "test  "_ts,
          "      "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_an_item, sends_item_changed_signal_when_an_item_is_selected)
{
    list_->select_item(0);
    ASSERT_TRUE(item_changed_);
}

TEST_F(a_list_with_an_item, reports_a_selected_item_as_selected)
{
    list_->select_item(0);

    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_an_item, signals_a_changed_cursor_position_when_an_item_is_selected)
{
    list_->select_item(0);
    ASSERT_TRUE(cursor_position_changed_);
}

TEST_F(a_list_with_an_item, selects_the_item_when_it_is_clicked)
{
    list_->event(
        terminalpp::mouse::event {
            terminalpp::mouse::event_type::left_button_down,
            { 0, 0 }
        });

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_an_item, selects_the_item_when_the_up_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_an_item, selects_the_item_when_the_down_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_an_item, selects_no_item_when_empty_space_is_clicked)
{
    list_->event(
        terminalpp::mouse::event {
            terminalpp::mouse::event_type::left_button_down,
            { 0, 1 }
        });

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

namespace {

class a_list_with_a_selected_item : public a_list_with_an_item
{
protected:
    a_list_with_a_selected_item()
    {
        list_->select_item(0);
        reset();
    }
};

}

TEST_F(a_list_with_a_selected_item, draws_that_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "\\p-test  "_ets,
          "      "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_a_selected_item, deselects_the_item_when_the_up_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_a_selected_item, deselects_the_item_when_the_down_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_a_selected_item, deselects_the_item_when_empty_space_is_clicked)
{
    list_->event(
        terminalpp::mouse::event {
            terminalpp::mouse::event_type::left_button_down,
            { 0, 1 }
        });

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
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
        terminalpp::coordinate_type(item_text1.size()),
        2
    };

    ASSERT_EQ(expected_preferred_size, list_->get_preferred_size());
}

TEST_F(a_list_with_two_items, draws_those_items)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "l0    "_ts,
          "line1 "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items, reports_a_selected_item_as_selected)
{
    list_->select_item(0);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);

    list_->select_item(1);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(1, *selected_item_index_);
}

TEST_F(a_list_with_two_items, sends_item_changed_signal_when_an_item_is_selected)
{
    list_->select_item(0);
    ASSERT_TRUE(item_changed_);

    item_changed_ = false;
    list_->select_item(1);
    ASSERT_TRUE(item_changed_);
}

TEST_F(a_list_with_two_items, selects_the_first_item_when_it_is_clicked)
{
    list_->event(
        terminalpp::mouse::event {
            terminalpp::mouse::event_type::left_button_down,
            { 0, 0 }
        });

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_two_items, selects_the_second_item_when_it_is_clicked)
{
    list_->event(
        terminalpp::mouse::event {
            terminalpp::mouse::event_type::left_button_down,
            { 0, 1 }
        });

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(1, *selected_item_index_);
}

TEST_F(a_list_with_two_items, selects_the_second_item_when_the_up_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(1, *selected_item_index_);
}

TEST_F(a_list_with_two_items, selects_the_first_item_when_the_down_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

namespace {

class a_list_with_two_items_and_the_first_selected : public a_list_with_two_items
{
protected:
    a_list_with_two_items_and_the_first_selected()
    {
        list_->select_item(0);
        reset();
    }
};

}

TEST_F(a_list_with_two_items_and_the_first_selected, draws_the_first_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "\\p-l0    "_ets,
          "line1 "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items_and_the_first_selected, redraws_the_items_when_the_item_is_deselected)
{
    list_->draw(surface_, {{}, list_size});
    list_->select_item(std::nullopt);

    assert_similar_canvas(
        { "l0    "_ts,
          "line1 "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items_and_the_first_selected, selects_the_second_item_when_the_down_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(1, *selected_item_index_);
}

TEST_F(a_list_with_two_items_and_the_first_selected, deselects_the_first_item_when_the_up_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
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
        reset();
    }
};

}

TEST_F(a_list_with_two_items_and_the_second_selected, draws_the_second_item_in_negative)
{
    list_->draw(surface_, {{}, list_size});

    assert_similar_canvas(
        { "l0    "_ts,
          "\\p-line1 "_ets,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items_and_the_second_selected, deselects_the_second_item_when_the_down_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_down});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_two_items_and_the_second_selected, selects_the_first_item_when_the_up_key_is_pressed)
{
    list_->event(
        terminalpp::virtual_key{terminalpp::vk::cursor_up});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_two_items_and_the_first_selected, redraws_the_items_when_the_second_is_selected)
{
    list_->draw(surface_, {{}, list_size});
    list_->select_item(1);

    assert_similar_canvas(
        { "l0    "_ts,
          "\\p-line1 "_ets,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items_and_the_second_selected, redraws_the_items_when_the_first_is_selected)
{
    list_->draw(surface_, {{}, list_size});
    list_->select_item(0);

    assert_similar_canvas(
        { "\\p-l0    "_ets,
          "line1 "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_two_items_and_the_second_selected, redraws_the_items_when_the_item_is_deselected)
{
    list_->draw(surface_, {{}, list_size});
    list_->select_item(std::nullopt);

    assert_similar_canvas(
        { "l0    "_ts,
          "line1 "_ts,
          "      "_ts },
        canvas_);
}

TEST_F(a_list_with_a_selected_item, when_items_are_set_to_empty_list_deselects_item)
{
    list_->set_items({});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_two_items_and_the_first_selected, when_items_are_set_to_empty_list_deselects_item)
{
    list_->set_items({});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_two_items_and_the_second_selected, when_items_are_set_to_empty_list_deselects_item)
{
    list_->set_items({});

    ASSERT_TRUE(item_changed_);
    ASSERT_FALSE(selected_item_index_.has_value());
}

TEST_F(a_list_with_a_selected_item, when_items_are_set_to_a_one_element_list_the_first_item_remains_selected)
{
    list_->set_items({"new item"_ts});

    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_two_items_and_the_second_selected, when_items_are_set_to_a_one_element_list_the_first_item_becomes_selected)
{
    list_->set_items({"new item"_ts});

    ASSERT_TRUE(item_changed_);
    ASSERT_TRUE(selected_item_index_.has_value());
    ASSERT_EQ(0, *selected_item_index_);
}

TEST_F(a_list_with_two_items_and_the_second_selected, has_a_cursor_position_on_the_second_row)
{
    ASSERT_EQ(terminalpp::point(0, 1), list_->get_cursor_position());
}
