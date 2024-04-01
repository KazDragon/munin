#include "mock/component.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <munin/horizontal_strip_layout.hpp>

using testing::Return;
using testing::StrictMock;

TEST(make_horizontal_strip_layout, creates_a_new_horizontal_strip_layout)
{
    auto lyt = munin::make_horizontal_strip_layout();
    auto hsl = dynamic_cast<munin::horizontal_strip_layout *>(lyt.get());
    ASSERT_TRUE(hsl != nullptr);
}

TEST(a_horizontal_strip_layout_with_no_components, has_a_preferred_size_of_zero)
{
    munin::horizontal_strip_layout hsl;
    auto preferred_size = hsl.get_preferred_size({}, {});

    ASSERT_EQ(terminalpp::extent{}, preferred_size);
}

TEST(
    a_horizontal_strip_layout_with_one_component,
    has_a_preferred_size_of_that_component)
{
    static constexpr terminalpp::extent comp_preferred_size{80, 24};
    auto comp = std::make_shared<StrictMock<mock_component>>();
    std::shared_ptr<munin::component> mcomp = comp;

    EXPECT_CALL(*comp, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));

    munin::horizontal_strip_layout hsl;
    auto preferred_size = hsl.get_preferred_size({mcomp}, {});

    ASSERT_EQ(comp_preferred_size, preferred_size);
}

TEST(
    a_horizontal_strip_layout_with_multiple_components,
    has_a_preferred_size_with_the_width_of_the_widest_and_height_of_the_sum_of_the_heights)
{
    static constexpr terminalpp::extent comp0_preferred_size{80, 24};
    static constexpr terminalpp::extent comp1_preferred_size{60, 13};
    static constexpr terminalpp::extent expected_preferred_size{80, 37};

    auto comp0 = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp0, do_get_preferred_size())
        .WillOnce(Return(comp0_preferred_size));

    auto comp1 = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp1, do_get_preferred_size())
        .WillOnce(Return(comp1_preferred_size));

    munin::horizontal_strip_layout hsl;
    auto preferred_size = hsl.get_preferred_size({comp0, comp1}, {});

    ASSERT_EQ(expected_preferred_size, preferred_size);
}

TEST(a_horizontal_strip_layout_with_no_components, lays_out_nothing)
{
    munin::horizontal_strip_layout hsl;
    hsl({}, {}, {80, 20});
}

TEST(
    a_horizontal_strip_layout_with_one_component,
    fills_width_but_not_height_with_component)
{
    static constexpr terminalpp::extent comp_preferred_size{80, 24};
    static constexpr terminalpp::extent layout_size{100, 40};
    static constexpr terminalpp::extent expected_size{100, 24};

    auto comp = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));

    EXPECT_CALL(*comp, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*comp, do_set_size(expected_size));

    munin::horizontal_strip_layout hsl;
    hsl({comp}, {}, layout_size);
}

TEST(
    a_horizontal_strip_layout_with_multiple_components,
    plots_components_below_each_other)
{
    // The layout will continue to plot components below each other, giving
    // them the maximum width, and ignoring the boundaries of the layout.
    static constexpr terminalpp::extent comp_preferred_size{80, 24};
    static constexpr terminalpp::extent layout_size{100, 40};
    static constexpr terminalpp::extent expected_size{100, 24};

    static constexpr terminalpp::point expected0_pos{0, 0};
    static constexpr terminalpp::point expected1_pos{0, 24};
    static constexpr terminalpp::point expected2_pos{0, 48};

    auto comp0 = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp0, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));

    auto comp1 = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp1, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));

    auto comp2 = std::make_shared<StrictMock<mock_component>>();
    EXPECT_CALL(*comp2, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));

    EXPECT_CALL(*comp0, do_set_position(expected0_pos));
    EXPECT_CALL(*comp0, do_set_size(expected_size));

    EXPECT_CALL(*comp1, do_set_position(expected1_pos));
    EXPECT_CALL(*comp1, do_set_size(expected_size));

    EXPECT_CALL(*comp2, do_set_position(expected2_pos));
    EXPECT_CALL(*comp2, do_set_size(expected_size));

    munin::horizontal_strip_layout hsl;
    hsl({comp0, comp1, comp2}, {}, layout_size);
}

TEST(a_horizontal_strip_layout, reports_attributes_as_json)
{
    munin::horizontal_strip_layout hsl;
    munin::layout &lyt = hsl;

    nlohmann::json json = lyt.to_json();
    ASSERT_EQ("horizontal_strip_layout", json["type"]);
}
