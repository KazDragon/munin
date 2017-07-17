#include <munin/grid_layout.hpp>
#include "mock/component.hpp"
#include <gtest/gtest.h>

using testing::Return;

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
