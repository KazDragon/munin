#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/compass_layout.hpp>
#include <terminalpp/rectangle.hpp>

using testing::Return;
using testing::ValuesIn;

static auto const north = std::any{munin::compass_layout::heading::north};
static auto const south = std::any{munin::compass_layout::heading::south};
static auto const east = std::any{munin::compass_layout::heading::east};
static auto const west = std::any{munin::compass_layout::heading::west};
static auto const centre = std::any{munin::compass_layout::heading::centre};

TEST(compass_layout_test, reports_attributes_as_json)
{
    munin::compass_layout cl;
    munin::layout &lyt = cl;

    nlohmann::json json = lyt.to_json();

    ASSERT_EQ("compass_layout", json["type"]);
}

TEST(a_compass_layout_with_no_elements, has_a_zero_preferred_size)
{
    munin::compass_layout layout;
    ASSERT_EQ(terminalpp::extent(0, 0), layout.get_preferred_size({}, {}));
}

using compass_layout_component_data = std::tuple<
    terminalpp::extent,    // preferred size
    std::any,              // layout hint
    terminalpp::rectangle  // expected size
    >;

using compass_layout_test_data = std::tuple<
    std::vector<compass_layout_component_data>,  // component data
    terminalpp::extent,                          // container size
    terminalpp::extent                           // preferred_size
    >;

class compass_layouts : public testing::TestWithParam<compass_layout_test_data>
{
};

TEST_P(compass_layouts, place_components_at_these_positions)
{
    auto const &param = GetParam();
    auto const &component_data = std::get<0>(param);
    auto const &container_size = std::get<1>(param);
    auto const &expected_preferred_size = std::get<2>(param);

    std::vector<std::shared_ptr<munin::component>> components;
    std::vector<std::any> hints;

    for (auto const &component_datum : component_data)
    {
        auto const &preferred_size = std::get<0>(component_datum);
        auto const &hint = std::get<1>(component_datum);
        auto const &expected_placement = std::get<2>(component_datum);

        auto component = std::make_shared<mock_component>();
        EXPECT_CALL(*component, do_get_preferred_size())
            .WillRepeatedly(Return(preferred_size));
        EXPECT_CALL(*component, do_set_position(expected_placement.origin_));
        EXPECT_CALL(*component, do_set_size(expected_placement.size_));

        components.push_back(component);
        hints.push_back(hint);
    }

    auto lyt = munin::make_compass_layout();

    ASSERT_EQ(
        expected_preferred_size, lyt->get_preferred_size(components, hints));
    (*lyt)(components, hints, container_size);
}

INSTANTIATE_TEST_SUITE_P(
    single_components_are_laid_out_on_their_edges,
    compass_layouts,
    ValuesIn({
        compass_layout_test_data{
                                 {compass_layout_component_data{
                {5, 5},
                std::any(munin::compass_layout::heading::centre),
                {{0, 0}, {10, 10}}}},
                                 {10, 10},
                                 {5, 5}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {5, 5},
                std::any(munin::compass_layout::heading::north),
                {{0, 0}, {10, 5}}}},
                                 {10, 10},
                                 {5, 5}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {5, 5},
                std::any(munin::compass_layout::heading::south),
                {{0, 5}, {10, 5}}}},
                                 {10, 10},
                                 {5, 5}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {5, 5},
                std::any(munin::compass_layout::heading::west),
                {{0, 0}, {5, 10}}}},
                                 {10, 10},
                                 {5, 5}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {5, 5},
                std::any(munin::compass_layout::heading::east),
                {{5, 0}, {5, 10}}}},
                                 {10, 10},
                                 {5, 5}}
}));

INSTANTIATE_TEST_SUITE_P(
    centre_components_occupy_unused_space,
    compass_layouts,
    ValuesIn({
        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::north),
                 {{0, 0}, {10, 5}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{0, 5}, {10, 5}}}},
                                 {10, 10},
                                 {5, 10}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::south),
                 {{0, 5}, {10, 5}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{0, 0}, {10, 5}}}},
                                 {10, 10},
                                 {5, 10}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 0}, {5, 10}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{5, 0}, {5, 10}}}},
                                 {10, 10},
                                 {10, 5}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::east),
                 {{5, 0}, {5, 10}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{0, 0}, {5, 10}}}},
                                 {10, 10},
                                 {10, 5}},
}));

INSTANTIATE_TEST_SUITE_P(
    squashed_components_shrink_to_fit,
    compass_layouts,
    ValuesIn({
        compass_layout_test_data{
                                 {compass_layout_component_data{
                {3, 3},
                std::any(munin::compass_layout::heading::centre),
                {{0, 0}, {2, 2}}}},
                                 {2, 2},
                                 {3, 3}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {3, 3},
                std::any(munin::compass_layout::heading::north),
                {{0, 0}, {2, 2}}}},
                                 {2, 2},
                                 {3, 3}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {3, 3},
                std::any(munin::compass_layout::heading::south),
                {{0, 0}, {2, 2}}}},
                                 {2, 2},
                                 {3, 3}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {3, 3},
                std::any(munin::compass_layout::heading::west),
                {{0, 0}, {2, 2}}}},
                                 {2, 2},
                                 {3, 3}},

        compass_layout_test_data{
                                 {compass_layout_component_data{
                {3, 3},
                std::any(munin::compass_layout::heading::east),
                {{0, 0}, {2, 2}}}},
                                 {2, 2},
                                 {3, 3}}
}));

INSTANTIATE_TEST_SUITE_P(
    order_of_insertion_takes_priority,
    compass_layouts,
    ValuesIn({
  /*
  +------------+
  |+----------+|
  ||    N     ||
  |+--+----+--+|
  ||  |    |  ||
  ||  |    |  ||
  ||W | C  | E||
  ||  |    |  ||
  ||  |    |  ||
  ||  |    |  ||
  |+--+----+--+|
  ||    S     ||
  |+----------+|
  +------------+
  */

        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::north),
                 {{0, 0}, {12, 3}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::south),
                 {{0, 9}, {12, 3}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::east),
                 {{9, 3}, {3, 6}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 3}, {3, 6}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{3, 3}, {6, 6}}}},
                                 {12, 12},
                                 {11, 11}},

 /*
  +------------+
  |+--+----+--+|
  ||  | N  |  ||
  ||  +----+  ||
  ||  |    |  ||
  ||  |    |  ||
  ||W | C  | E||
  ||  |    |  ||
  ||  |    |  ||
  ||  |    |  ||
  ||  +----+  ||
  ||  | S  |  ||
  |+--+----+--+|
  +------------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 0}, {3, 12}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::east),
                 {{9, 0}, {3, 12}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::north),
                 {{3, 0}, {6, 3}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::south),
                 {{3, 9}, {6, 3}}},
             compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{3, 3}, {6, 6}}}},
                                 {12, 12},
                                 {11, 11}},

 /*
  +------------+
  |+--+----+--+|
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  ||    W     ||  And everything else is underneath
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  |+--+----+--+|
  +------------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 0}, {12, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::east),
                 {{0, 0}, {0, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::north),
                 {{12, 0}, {0, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::south),
                 {{12, 0}, {0, 0}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::centre),
                 {{12, 12}, {0, 0}}}},
                                 {12, 12},
                                 {36, 36}},

 /*
  +------------+
  |+--+----+--+|
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  ||    E     ||  And everything else is underneath
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  ||          ||
  |+--+----+--+|
  +------------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::east),
                 {{0, 0}, {12, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 0}, {0, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::north),
                 {{0, 0}, {0, 12}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::south),
                 {{0, 0}, {0, 0}}},
             compass_layout_component_data{
                 {12, 12},
                 std::any(munin::compass_layout::heading::centre),
                 {{0, 12}, {0, 0}}}},
                                 {12, 12},
                                 {36, 36}}
}));

INSTANTIATE_TEST_SUITE_P(
    centred_components_are_laid_out_last,
    compass_layouts,
    ValuesIn({
        compass_layout_test_data{
                                 {compass_layout_component_data{
                 {5, 5},
                 std::any(munin::compass_layout::heading::centre),
                 {{3, 3}, {6, 6}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::north),
                 {{0, 0}, {12, 3}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::south),
                 {{0, 9}, {12, 3}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::east),
                 {{9, 3}, {3, 6}}},
             compass_layout_component_data{
                 {3, 3},
                 std::any(munin::compass_layout::heading::west),
                 {{0, 3}, {3, 6}}}},
                                 {12, 12},
                                 {11, 11}}
}));

INSTANTIATE_TEST_SUITE_P(
    compass_layouts_preserve_empty_space,
    compass_layouts,
    ValuesIn({
  /*
  L-shapes should preserve the associated empty space.
  For example:
  
  +--------+
  |    N   |
  +------+-+
  | |
  |E|
  SPACE! | |
  | |
  +-+
  
  And rotations/permutations thereof.
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}},
             compass_layout_component_data{{1, 3}, east, {{2, 1}, {1, 3}}}},
                                 {3, 4},
                                 {3, 4}},

 /*
  +-+
  | |
  |E|
  | |
  | |
  +--------+-+
  |    S     |
  +----------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{3, 1}, south, {{0, 3}, {3, 1}}},
             compass_layout_component_data{{1, 3}, east, {{2, 0}, {1, 3}}}},
                                 {3, 4},
                                 {3, 4}},

 /*
  +-+
  | |
  |W|
  | |
  | |
  +-+--------+
  |    S     |
  +----------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{3, 1}, south, {{0, 3}, {3, 1}}},
             compass_layout_component_data{{1, 3}, west, {{0, 0}, {1, 3}}}},
                                 {3, 4},
                                 {3, 4}},

 /*
  +----------+
  |    N     |
  +-+--------+
  | |
  | |
  |W|
  | |
  +-+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}},
             compass_layout_component_data{{1, 3}, west, {{0, 1}, {1, 3}}}},
                                 {3, 4},
                                 {3, 4}},

 /*
  +-----+-+
  |  N  | |
  +-----+ |
  | |
  SPACE!|E|
  | |
  +-+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{1, 3}, east, {{3, 0}, {1, 3}}},
             compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}}},
                                 {4, 3},
                                 {4, 3}},

 /*
  Adding a small centre component will grow to fit the available space.
  
  +-----+-+
  |  N  | |
  +-+---+ |
  |C|   | |
  +-+   |E|
  |  \_ | |
  +----\+-+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{1, 3}, east, {{3, 0}, {1, 3}}},
             compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}},
             compass_layout_component_data{{1, 1}, centre, {{0, 1}, {3, 2}}}},
                                 {4, 3},
                                 {4, 3}},

 /*
  Adding a perfectly fitting centre component fill the available space.
  
  +-----+-+
  |  N  | |
  +-----+ |
  |     | |
  |  C  |E|
  |     | |
  +-----+-+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{1, 3}, east, {{3, 0}, {1, 3}}},
             compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}},
             compass_layout_component_data{{3, 2}, centre, {{0, 1}, {3, 2}}}},
                                 {4, 3},
                                 {4, 3}},

 /*
  Adding a large centre component shrink into the available space,
  even if its preferred size is large.
  
  +-+-----+-+
  |\|  N  | |
  | +-----+ |
  | |     | |
  | |  C  |E|
  | |     | |
  | +-----+-+
  |/       \|
  +---------+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{1, 3}, east, {{3, 0}, {1, 3}}},
             compass_layout_component_data{{3, 1}, north, {{0, 0}, {3, 1}}},
             compass_layout_component_data{{5, 5}, centre, {{0, 1}, {3, 2}}}},
                                 {4, 3},
                                 {6, 6}},

 /*
  Finally, a large centre component causes other components to stretch
  into the available space, if it is given.
  
  +-+-----+-+
  |-|  N  | |
  +-+-----+ |
  |       | |
  |    C  |E|
  |       | |
  |       +-+
  |       |||
  +-------+-+
  */
        compass_layout_test_data{
                                 {compass_layout_component_data{{1, 3}, east, {{5, 0}, {1, 6}}},
             compass_layout_component_data{{3, 1}, north, {{0, 0}, {5, 1}}},
             compass_layout_component_data{{5, 5}, centre, {{0, 1}, {5, 5}}}},
                                 {6, 6},
                                 {6, 6}},
}));
