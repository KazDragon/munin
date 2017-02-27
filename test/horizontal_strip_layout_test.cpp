#include <munin/horizontal_strip_layout.hpp>
#include <munin/context.hpp>
#include "mock/component.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Return;
using testing::StrictMock;

TEST(a_horizontal_strip_layout_with_no_components, has_a_preferred_size_of_zero)
{
    munin::horizontal_strip_layout hsl;
    auto preferred_size = hsl.get_preferred_size({}, {});
    
    ASSERT_EQ(terminalpp::extent{}, preferred_size);
}

TEST(a_horizontal_strip_layout_with_one_component, has_a_preferred_size_of_that_component)
{
    static constexpr terminalpp::extent comp_preferred_size{80, 24}; 
    std::shared_ptr<StrictMock<component>> comp = 
        std::make_shared<StrictMock<component>>();
    std::shared_ptr<munin::component> mcomp = comp;

    EXPECT_CALL(*comp, do_get_preferred_size())
        .WillOnce(Return(comp_preferred_size));
        
    munin::horizontal_strip_layout hsl;
    auto preferred_size = hsl.get_preferred_size({mcomp}, {});
    
    ASSERT_EQ(comp_preferred_size, preferred_size);
}
