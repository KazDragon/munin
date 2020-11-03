#include <munin/list.hpp>
#include <terminalpp/string.hpp>
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

TEST_F(a_new_list, when_setting_the_selected_item_has_no_selected_item)
{
    list_->set_selected_item_index(4);
    auto const selected_item_index = list_->get_selected_item_index();
    ASSERT_FALSE(selected_item_index.is_initialized());
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
