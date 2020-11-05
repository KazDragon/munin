#include <munin/list.hpp>
#include <munin/render_surface.hpp>
#include <terminalpp/canvas.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
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
    std::shared_ptr<munin::list> list_ { munin::make_list() };
};

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
    list_->set_size({3, 3});

    terminalpp::canvas canvas({6, 3});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    list_->draw(surface, {{}, list_->get_size()});

    ASSERT_EQ(terminalpp::element{' '}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][2]);
}

TEST_F(a_new_list, requests_a_redraw_when_setting_the_list_items)
{
    auto const list_size = terminalpp::extent{5, 13};
    list_->set_size(list_size);

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

namespace {

class a_list_with_an_item : public a_new_list
{
public:
    a_list_with_an_item()
    {
        list_->set_items({item_text});
    }

protected:
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
    list_->set_size({6, 3});

    terminalpp::canvas canvas({6, 3});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    list_->draw(surface, {{}, list_->get_size()});

    ASSERT_EQ(terminalpp::element{'t'}, canvas[0][0]);
    ASSERT_EQ(terminalpp::element{'e'}, canvas[1][0]);
    ASSERT_EQ(terminalpp::element{'s'}, canvas[2][0]);
    ASSERT_EQ(terminalpp::element{'t'}, canvas[3][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][0]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][1]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[0][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[1][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[2][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[3][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[4][2]);
    ASSERT_EQ(terminalpp::element{' '}, canvas[5][2]);
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

namespace {

class a_list_with_a_selected_item : public a_list_with_an_item
{
protected:
    a_list_with_a_selected_item()
    {
        list_->select_item(0);
    }
};

TEST_F(a_list_with_a_selected_item, draws_that_item_in_negative)
{
    list_->set_size({6, 3});

    terminalpp::canvas canvas({6, 3});

    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [](terminalpp::element &elem,
           terminalpp::coordinate_type column,
           terminalpp::coordinate_type row)
        {
            elem = 'X';
        });

    munin::render_surface surface{canvas};
    list_->draw(surface, {{}, list_->get_size()});

    auto const negative_attr = []{
        terminalpp::attribute attr;
        attr.polarity_ = terminalpp::ansi::graphics::polarity::negative;
        return attr;
    }();

    ASSERT_EQ(terminalpp::element('t', negative_attr), canvas[0][0]);
    ASSERT_EQ(terminalpp::element('e', negative_attr), canvas[1][0]);
    ASSERT_EQ(terminalpp::element('s', negative_attr), canvas[2][0]);
    ASSERT_EQ(terminalpp::element('t', negative_attr), canvas[3][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas[4][0]);
    ASSERT_EQ(terminalpp::element(' ', negative_attr), canvas[5][0]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[3][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[4][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[5][1]);
    ASSERT_EQ(terminalpp::element(' '), canvas[0][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[1][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[2][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[3][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[4][2]);
    ASSERT_EQ(terminalpp::element(' '), canvas[5][2]);
}

}

