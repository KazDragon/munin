#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/aligned_layout.hpp>
#include <munin/container.hpp>

using testing::Return;
using testing::ValuesIn;

TEST(aligned_layout_test, reports_attributes_as_json)
{
    munin::aligned_layout al;
    munin::layout &lyt = al;

    nlohmann::json json = lyt.to_json();

    ASSERT_EQ("aligned_layout", json["type"]);
}

TEST(
    aligned_layout_test,
    preferred_size_is_maximum_with_and_height_of_components)
{
    auto component0 = std::make_shared<mock_component>();
    EXPECT_CALL(*component0, do_get_preferred_size())
        .WillRepeatedly(Return(terminalpp::extent{10, 5}));

    auto component1 = std::make_shared<mock_component>();
    EXPECT_CALL(*component1, do_get_preferred_size())
        .WillRepeatedly(Return(terminalpp::extent{7, 12}));

    auto lyt = munin::make_aligned_layout();

    auto const preferred_size = lyt->get_preferred_size(
        std::vector<std::shared_ptr<munin::component>>{component0, component1},
        std::vector<boost::any>{munin::alignment_hcvb, munin::alignment_hcvt});

    auto const expected_size = terminalpp::extent{10, 12};

    ASSERT_EQ(expected_size, preferred_size);
}

using aligned_layout_test_data = std::tuple<
    munin::alignment,      // alignment of component under test
    terminalpp::extent,    // container size
    terminalpp::extent,    // component size
    terminalpp::rectangle  // expected placement
    >;

class aligned_layouts : public testing::TestWithParam<aligned_layout_test_data>
{
};

TEST_P(aligned_layouts, place_components_at_these_positions)
{
    auto const &param = GetParam();
    auto const &alignment = std::get<0>(param);
    auto const &container_size = std::get<1>(param);
    auto const &component_size = std::get<2>(param);
    auto const &expected_placement = std::get<3>(param);

    auto component = std::make_shared<mock_component>();
    EXPECT_CALL(*component, do_get_preferred_size())
        .WillRepeatedly(Return(component_size));
    EXPECT_CALL(*component, do_set_position(expected_placement.origin_));
    EXPECT_CALL(*component, do_set_size(expected_placement.size_));

    auto lyt = munin::make_aligned_layout();

    (*lyt)({component}, {alignment}, container_size);
}

INSTANTIATE_TEST_SUITE_P(
    laying_out_evenly_spaced_single_components,
    aligned_layouts,
    ValuesIn(std::vector<aligned_layout_test_data>{
        aligned_layout_test_data{
                                 munin::alignment_hlvt, {10, 10}, {4, 4}, {{0, 0}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hcvt, {10, 10}, {4, 4}, {{3, 0}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hrvt, {10, 10}, {4, 4}, {{6, 0}, {4, 4}}  },

        aligned_layout_test_data{
                                 munin::alignment_hlvc, {10, 10}, {4, 4}, {{0, 3}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hcvc, {10, 10}, {4, 4}, {{3, 3}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hrvc, {10, 10}, {4, 4}, {{6, 3}, {4, 4}}  },

        aligned_layout_test_data{
                                 munin::alignment_hlvb, {10, 10}, {4, 4}, {{0, 6}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hcvb, {10, 10}, {4, 4}, {{3, 6}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_hrvb, {10, 10}, {4, 4}, {{6, 6}, {4, 4}}  },
        aligned_layout_test_data{
                                 munin::alignment_fill, {10, 10}, {4, 4}, {{0, 0}, {10, 10}}},
}));

INSTANTIATE_TEST_SUITE_P(
    laying_out_unevenly_spaced_single_components,
    aligned_layouts,
    ValuesIn(std::vector<aligned_layout_test_data>{
        aligned_layout_test_data{
                                 munin::alignment_hlvt, {9, 9}, {4, 4}, {{0, 0}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvt, {9, 9}, {4, 4}, {{2, 0}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvt, {9, 9}, {4, 4}, {{5, 0}, {4, 4}}},

        aligned_layout_test_data{
                                 munin::alignment_hlvc, {9, 9}, {4, 4}, {{0, 2}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvc, {9, 9}, {4, 4}, {{2, 2}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvc, {9, 9}, {4, 4}, {{5, 2}, {4, 4}}},

        aligned_layout_test_data{
                                 munin::alignment_hlvb, {9, 9}, {4, 4}, {{0, 5}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvb, {9, 9}, {4, 4}, {{2, 5}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvb, {9, 9}, {4, 4}, {{5, 5}, {4, 4}}},
        aligned_layout_test_data{
                                 munin::alignment_fill, {9, 9}, {4, 4}, {{0, 0}, {9, 9}}},
}));

INSTANTIATE_TEST_SUITE_P(
    laying_out_squashed_single_components,
    aligned_layouts,
    ValuesIn(std::vector<aligned_layout_test_data>{
        aligned_layout_test_data{
                                 munin::alignment_hlvt, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvt, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvt, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},

        aligned_layout_test_data{
                                 munin::alignment_hlvc, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvc, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvc, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},

        aligned_layout_test_data{
                                 munin::alignment_hlvb, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hcvb, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}},
        aligned_layout_test_data{
                                 munin::alignment_hrvb, {3, 3}, {4, 4}, {{0, 0}, {3, 3}}}
}));
