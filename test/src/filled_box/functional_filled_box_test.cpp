#include <munin/filled_box.hpp>
#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

TEST(a_filled_box, can_retrieve_its_fill_from_a_function)
{
    terminalpp::element fill = 'x';
    
    auto fill_function = [&fill](munin::render_surface &)
    {
        return fill;
    };
    
    auto box = munin::make_fill(fill_function);
    box->set_size({2, 2});
    
    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface(canvas);
    
    box->draw(surface, terminalpp::rectangle{{}, {2, 2}});
    
    ASSERT_EQ('x', canvas[0][0]);
    ASSERT_EQ('x', canvas[0][1]);
    ASSERT_EQ('x', canvas[1][0]);
    ASSERT_EQ('x', canvas[1][1]);
    
    fill = 'y';

    box->draw(surface, terminalpp::rectangle{{}, {2, 2}});
    
    ASSERT_EQ('y', canvas[0][0]);
    ASSERT_EQ('y', canvas[0][1]);
    ASSERT_EQ('y', canvas[1][0]);
    ASSERT_EQ('y', canvas[1][1]);

}