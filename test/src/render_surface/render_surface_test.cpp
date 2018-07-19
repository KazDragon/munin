#include <munin/render_surface.hpp>
#include <gtest/gtest.h>

TEST(render_surface_test, default_render_surface_views_same_basis_as_canvas)
{
    terminalpp::canvas canvas({2, 2});
    munin::render_surface render_surface(canvas);

    render_surface[0][0] = 'a';
    render_surface[0][1] = 'b';
    render_surface[1][0] = 'c';
    render_surface[1][1] = 'd';

    ASSERT_TRUE(canvas[0][0] == 'a');
    ASSERT_TRUE(canvas[0][1] == 'b');
    ASSERT_TRUE(canvas[1][0] == 'c');
    ASSERT_TRUE(canvas[1][1] == 'd');
}

TEST(render_surface_test, default_render_surface_has_same_size_as_canvas)
{
    terminalpp::canvas canvas({2, 2});
    munin::render_surface render_surface(canvas);

    ASSERT_EQ(canvas.size(), render_surface.size());
}

TEST(render_surface_test, offset_render_surface_has_offset_basis)
{
    terminalpp::canvas canvas({2, 2});
    munin::render_surface render_surface(canvas);

    render_surface.offset_by({1, 1});
    render_surface[0][0] = 'x';
    ASSERT_TRUE(render_surface[0][0] == 'x');

    ASSERT_TRUE(canvas[0][0] == ' ');
    ASSERT_TRUE(canvas[0][1] == ' ');
    ASSERT_TRUE(canvas[1][0] == ' ');
    ASSERT_TRUE(canvas[1][1] == 'x');
}

TEST(render_surface_test, offset_render_surface_has_reduced_size)
{
    terminalpp::canvas canvas({2, 2});
    munin::render_surface render_surface(canvas);

    render_surface.offset_by({1, 1});

    ASSERT_EQ(
        (canvas.size() - terminalpp::extent{1, 1}),
        render_surface.size());
}

TEST(render_surface_test, offset_is_cumulative)
{
    terminalpp::canvas canvas({3, 3});
    munin::render_surface render_surface(canvas);

    render_surface.offset_by({1, 1});
    render_surface.offset_by({1, 1});

    render_surface[0][0] = 'x';
    ASSERT_TRUE(canvas[2][2] == 'x');
}
