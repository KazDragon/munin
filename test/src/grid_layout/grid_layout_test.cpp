#include <munin/grid_layout.hpp>
#include "mock/component.hpp"
#include <tuple>
#include <gtest/gtest.h>

using testing::Return;
using testing::ValuesIn;

TEST(grid_layout_test, grid_layout_is_layout)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{1, 1});
}

TEST(grid_layout_test, preferred_size_of_a_layout_with_no_views_is_zero)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{1, 1});

    auto const expected = terminalpp::extent{0, 0};
    auto const result   = lyt->get_preferred_size({}, {});
    
    ASSERT_EQ(expected, result);
}

TEST(grid_layout_test, preferred_size_of_a_1x1_grid_with_one_view_is_the_preferred_size_of_the_view)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{1, 1});

    auto const expected = terminalpp::extent{5, 5};
    
    auto component = std::make_shared<mock_component>();
    EXPECT_CALL(*component, do_get_preferred_size())
        .WillRepeatedly(Return(expected));
        
    auto result = lyt->get_preferred_size({component}, {{}});
    
    ASSERT_EQ(expected, result);
}

TEST(grid_layout_test, preferred_size_of_a_2x1_grid_with_two_views_is_the_twice_the_maximum_width_and_the_maximum_height)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{2, 1});
        
    auto view0_preferred_size = terminalpp::extent{3, 5};
    auto view1_preferred_size = terminalpp::extent{5, 3};
    auto expected = terminalpp::extent{10, 5};

    auto component0 = std::make_shared<mock_component>();
    EXPECT_CALL(*component0, do_get_preferred_size())
        .WillRepeatedly(Return(view0_preferred_size));

    auto component1 = std::make_shared<mock_component>();
    EXPECT_CALL(*component1, do_get_preferred_size())
        .WillRepeatedly(Return(view1_preferred_size));

    auto result = lyt->get_preferred_size(
        { component0, component1 },
        { {},         {}         });
    
    ASSERT_EQ(expected, result);
}

TEST(grid_layout_test, preferred_size_of_a_1x2_grid_with_two_views_is_the_maximum_width_and_twice_the_maximum_height)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{1, 2});
        
    auto view0_preferred_size = terminalpp::extent{3, 5};
    auto view1_preferred_size = terminalpp::extent{5, 3};
    auto expected = terminalpp::extent{5, 10};
    
    auto component0 = std::make_shared<mock_component>();
    EXPECT_CALL(*component0, do_get_preferred_size())
        .WillRepeatedly(Return(view0_preferred_size));

    auto component1 = std::make_shared<mock_component>();
    EXPECT_CALL(*component1, do_get_preferred_size())
        .WillRepeatedly(Return(view1_preferred_size));

    auto result = lyt->get_preferred_size(
        { component0, component1 },
        { {},         {}         });
}

TEST(grid_layout_test, preferred_size_of_a_2x2_grid_with_four_views_is_twice_the_maximum_width_and_twice_the_maximum_height)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{2, 2});
        
    auto view0_preferred_size = terminalpp::extent{3, 5};
    auto view1_preferred_size = terminalpp::extent{5, 3};
    auto view2_preferred_size = terminalpp::extent{2, 6};
    auto view3_preferred_size = terminalpp::extent{4, 8};
    
    auto expected = terminalpp::extent{10, 16};

    auto component0 = std::make_shared<mock_component>();
    EXPECT_CALL(*component0, do_get_preferred_size())
        .WillRepeatedly(Return(view0_preferred_size));

    auto component1 = std::make_shared<mock_component>();
    EXPECT_CALL(*component1, do_get_preferred_size())
        .WillRepeatedly(Return(view1_preferred_size));

    auto component2 = std::make_shared<mock_component>();
    EXPECT_CALL(*component2, do_get_preferred_size())
        .WillRepeatedly(Return(view2_preferred_size));

    auto component3 = std::make_shared<mock_component>();
    EXPECT_CALL(*component3, do_get_preferred_size())
        .WillRepeatedly(Return(view3_preferred_size));

    auto result = lyt->get_preferred_size(
        { component0, component1, component2, component3 },
        { {},         {},         {},         {}         });
    
    ASSERT_EQ(expected, result);
}

TEST(grid_layout_test, laying_out_empty_grid_does_nothing)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{2, 2});

    (*lyt)({}, {}, {});    
}

TEST(grid_layout_test, layout_of_1x1_grid_with_component_lays_out_component_to_that_size)
{
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(
        terminalpp::extent{1, 1});

    auto const size = terminalpp::extent{4, 5};
    
    auto component = std::make_shared<mock_component>();
    
    EXPECT_CALL(*component, do_set_position(terminalpp::point(0, 0)));
    EXPECT_CALL(*component, do_set_size(terminalpp::extent(4, 5)));
    
    (*lyt)({component}, {}, size);
}

using bounds_data = std::tuple<terminalpp::point, terminalpp::extent>;

using grid_layout_test_data = std::tuple<
    terminalpp::extent, // grid dimensions
    terminalpp::extent, // container size
    std::vector<bounds_data> // position/size
>;

class laying_out_grids
  : public testing::TestWithParam<grid_layout_test_data>
{
};

TEST_P(laying_out_grids, at_these_positions)
{
    auto const &param = GetParam();
    auto const &grid_size = std::get<0>(param);
    auto const &container_size = std::get<1>(param);
    auto const &expected_results = std::get<2>(param);
    
    std::unique_ptr<munin::layout> lyt = munin::make_grid_layout(grid_size);
    
    std::vector<std::shared_ptr<munin::component>> components;
    
    for (auto const &expected_result : expected_results)
    {
        auto component = std::make_shared<mock_component>();
        EXPECT_CALL(*component, do_set_position(std::get<0>(expected_result)));
        EXPECT_CALL(*component, do_set_size(std::get<1>(expected_result)));
        
        components.push_back(component);
    }
    
    (*lyt)(components, {}, container_size);
}

INSTANTIATE_TEST_CASE_P(
    grid_where_the_size_is_divisible_by_the_dimensions_lays_components_out_evenly,
    laying_out_grids,
    ValuesIn(std::vector<grid_layout_test_data>{
        grid_layout_test_data{
            {1, 1},
            {10, 20},
            {
                bounds_data{ {0, 0}, {10, 20} }
            }
        },
        grid_layout_test_data{ 
            {2, 1},
            {4, 5},
            { 
                bounds_data{ {0, 0}, {2, 5} }, 
                bounds_data{ {2, 0}, {2, 5} }
            }
        },
        grid_layout_test_data{ 
            {1, 2},
            {5, 4},
            { 
                bounds_data{ {0, 0}, {5, 2} }, 
                bounds_data{ {0, 2}, {5, 2} }
            }
        },
        grid_layout_test_data{ 
            {3, 2},
            {9, 8},
            { 
                bounds_data{ {0, 0}, {3, 4} }, 
                bounds_data{ {3, 0}, {3, 4} }, 
                bounds_data{ {6, 0}, {3, 4} }, 
                bounds_data{ {0, 4}, {3, 4} }, 
                bounds_data{ {3, 4}, {3, 4} }, 
                bounds_data{ {6, 4}, {3, 4} }
            }
        }
    })
);

INSTANTIATE_TEST_CASE_P(
    grid_where_the_size_is_not_divisible_by_the_dimensions_distributes_remainder_over_components,
    laying_out_grids,
    ValuesIn(std::vector<grid_layout_test_data>{
        grid_layout_test_data{
            {2, 1},
            {11, 10},
            {
                bounds_data{ {0, 0}, {6, 10} },
                bounds_data{ {6, 0}, {5, 10} }
            }
        },
        grid_layout_test_data{
            {1, 2},
            {10, 11},
            {
                bounds_data{ {0, 0}, {10, 6} },
                bounds_data{ {0, 6}, {10, 5} }
            }
        },
        grid_layout_test_data{
            {3, 3},
            {14, 17},
            {
                bounds_data{ {0,  0},  {5, 6} },
                bounds_data{ {5,  0},  {5, 6} },
                bounds_data{ {10, 0},  {4, 6} },
                bounds_data{ {0,  6},  {5, 6} },
                bounds_data{ {5,  6},  {5, 6} },
                bounds_data{ {10, 6},  {4, 6} },
                bounds_data{ {0,  12}, {5, 5} },
                bounds_data{ {5,  12}, {5, 5} },
                bounds_data{ {10, 12}, {4, 5} }
            }
        }        
    })
);
