#include "mock/component.hpp"
#include "mock/frame.hpp"
#include <munin/filled_box.hpp>
#include <munin/frame.hpp>
#include <munin/framed_component.hpp>
#include <munin/render_surface.hpp>
#include <munin/solid_frame.hpp>
#include <gtest/gtest.h>

using testing::Return;
using testing::_;

TEST(a_new_framed_component, is_constructed_from_a_frame_and_a_component)
{
    std::shared_ptr<munin::framed_component> comp = munin::make_framed_component(
        make_mock_frame(),
        munin::make_fill('X')
    );
}

TEST(a_zero_size_framed_component, positions_subcomponents_at_origin)
{
    auto mock_frame = make_mock_frame();
    auto mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, north_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{0}));
    EXPECT_CALL(*mock_frame, south_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{0}));
    EXPECT_CALL(*mock_frame, west_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{0}));
    EXPECT_CALL(*mock_frame, east_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{0}));
        
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

    std::shared_ptr<mock_frame> mock_frame = make_mock_frame();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, north_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, south_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, west_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, east_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));

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

    std::shared_ptr<mock_frame> mock_frame = make_mock_frame();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, north_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, south_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, west_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, east_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));

    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{5, 0}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{1, 0}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{3, 0}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({5, 0});
}

TEST(a_framed_component_larger_than_the_frame, positions_the_inner_component_inward_of_the_frame)
{
    std::shared_ptr<mock_frame> mock_frame = make_mock_frame();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, north_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, south_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, west_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, east_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{1}));

    EXPECT_CALL(*mock_frame, do_set_position(terminalpp::point{0, 0}));
    EXPECT_CALL(*mock_frame, do_set_size(terminalpp::extent{5, 7}));
    EXPECT_CALL(*mock_comp,  do_set_position(terminalpp::point{1, 1}));
    EXPECT_CALL(*mock_comp,  do_set_size(terminalpp::extent{3, 5}));
    
    framed_component->set_position({0, 0});
    framed_component->set_size({5, 7});
}

TEST(a_framed_component, has_a_preferred_size_of_the_sum_of_the_frame_borders_and_inner)
{
    std::shared_ptr<mock_frame> mock_frame = make_mock_frame();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);
        
    EXPECT_CALL(*mock_frame, north_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{1}));
    EXPECT_CALL(*mock_frame, south_border_height())
        .WillOnce(Return(terminalpp::coordinate_type{2}));
    EXPECT_CALL(*mock_frame, west_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{3}));
    EXPECT_CALL(*mock_frame, east_border_width())
        .WillOnce(Return(terminalpp::coordinate_type{4}));

    EXPECT_CALL(*mock_comp, do_get_preferred_size())
        .WillOnce(Return(terminalpp::extent(7, 13)));

    ASSERT_EQ(
        terminalpp::extent(14, 16),
        framed_component->get_preferred_size());
}

TEST(a_framed_component, draws_the_inner_component_inside_the_frame)
{
    static constexpr terminalpp::attribute default_attribute;
    static constexpr terminalpp::attribute fill_attribute{
        terminalpp::graphics::colour::green
    };
    
    std::shared_ptr<munin::framed_component> comp = munin::make_framed_component(
        munin::make_solid_frame(),
        munin::make_fill({'X', fill_attribute})
    );
    
    comp->set_position({0, 0});
    comp->set_size({4, 4});
    
    terminalpp::canvas cvs({4, 4});
    munin::render_surface surface{cvs};
    
    comp->draw(surface, {{0, 0}, {4, 4}});
    
    ASSERT_EQ(default_attribute, cvs[0][0].attribute_);
    ASSERT_EQ(default_attribute, cvs[1][0].attribute_);
    ASSERT_EQ(default_attribute, cvs[2][0].attribute_);
    ASSERT_EQ(default_attribute, cvs[3][0].attribute_);
    ASSERT_EQ(default_attribute, cvs[0][1].attribute_);
    ASSERT_EQ(fill_attribute,    cvs[1][1].attribute_);
    ASSERT_EQ(fill_attribute,    cvs[2][1].attribute_);
    ASSERT_EQ(default_attribute, cvs[3][1].attribute_);
    ASSERT_EQ(default_attribute, cvs[0][2].attribute_);
    ASSERT_EQ(fill_attribute,    cvs[1][2].attribute_);
    ASSERT_EQ(fill_attribute,    cvs[2][2].attribute_);
    ASSERT_EQ(default_attribute, cvs[3][2].attribute_);
    ASSERT_EQ(default_attribute, cvs[0][3].attribute_);
    ASSERT_EQ(default_attribute, cvs[1][3].attribute_);
    ASSERT_EQ(default_attribute, cvs[2][3].attribute_);
    ASSERT_EQ(default_attribute, cvs[3][3].attribute_);
}

TEST(a_framed_component, forwards_events_to_the_inner_component)
{
    std::shared_ptr<mock_frame> mock_frame = make_mock_frame();
    std::shared_ptr<mock_component> mock_comp  = make_mock_component();
    
    auto framed_component = munin::make_framed_component(
        mock_frame,
        mock_comp);

    static std::string const value = "test";
    std::string received_value;

    EXPECT_CALL(*mock_comp, do_event(_))
        .WillOnce([&received_value](boost::any const &ev) {
            received_value = boost::any_cast<std::string>(ev);
        });

    framed_component->set_focus();
    ON_CALL(*mock_comp, do_has_focus)
        .WillByDefault(Return(true));

    framed_component->event(value);
    ASSERT_EQ(value, received_value);

}