#include "mock/component.hpp"
#include <munin/filled_box.hpp>
#include <munin/framed_component.hpp>
#include <munin/solid_frame.hpp>
#include <gtest/gtest.h>

using testing::Return;

TEST(a_new_framed_component, is_constructed_from_a_frame_and_a_component)
{
    std::shared_ptr<munin::framed_component> comp = munin::make_framed_component(
        munin::make_solid_frame(),
        munin::make_fill('X')
    );
}

TEST(a_zero_size_framed_component, positions_subcomponents_at_origin)
{
    std::shared_ptr<mock_component> mock_frame = make_mock_component();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{0, 0}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{0, 0}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({0, 0});
}

TEST(a_zero_width_framed_component, positions_inner_component_in_an_inner_row)
{
    // Despite it also being a zero-width component, the frame should be
    // respected as much as possible when positioning the inner component.

    std::shared_ptr<mock_component> mock_frame = make_mock_component();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{0, 5}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{0, 1}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{0, 3}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({0, 5});
}

TEST(a_zero_height_framed_component, positions_inner_component_in_an_inner_column)
{
    // Despite it also being a zero-height component, the frame should be
    // respected as much as possible when positioning the inner component.

    std::shared_ptr<mock_component> mock_frame = make_mock_component();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{5, 0}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{1, 0}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{3, 0}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({5, 0});
}

TEST(a_framed_component_larger_than_the_frame, positions_the_inner_component_inward_of_the_frame)
{
    std::shared_ptr<mock_component> mock_frame = make_mock_component();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{5, 7}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{1, 1}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{3, 5}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({5, 7});
}

TEST(a_frame_component, has_a_preferred_size_of_the_sum_of_the_frame_and_inner)
{
    std::shared_ptr<mock_component> mock_frame = make_mock_component();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, do_get_preferred_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));
    EXPECT_CALL(*mock_comp, do_get_preferred_size())
        .WillOnce(Return(terminalpp::extent(7, 13)));

    ASSERT_EQ(
        terminalpp::extent(9, 15),
        framed_component->get_preferred_size());
}
