#include "container_test.hpp"

#include <munin/render_surface.hpp>

using testing::_;
using testing::Return;

TEST_F(a_container, draws_subcomponent_when_drawn)
{
    auto component = std::make_shared<mock_component>();

    container_.set_size({2, 2});

    container_.add_component(component);

    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface{canvas};

    EXPECT_CALL(*component, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 0)));
    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));

    EXPECT_CALL(*component, do_draw(_, terminalpp::rectangle({0, 0}, {2, 2})));

    container_.draw(surface, terminalpp::rectangle({0, 0}, {2, 2}));
}

TEST_F(a_container, draws_partial_subcomponents_when_partially_drawn)
{
    auto component = std::make_shared<mock_component>();

    container_.set_size({2, 2});

    container_.add_component(component);

    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface{canvas};

    EXPECT_CALL(*component, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 0)));
    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));

    EXPECT_CALL(*component, do_draw(_, terminalpp::rectangle({1, 0}, {1, 2})));

    container_.draw(surface, terminalpp::rectangle({1, 0}, {1, 2}));
}

TEST_F(a_container, does_not_draw_subcomponents_outside_of_draw_region)
{
    auto component = std::make_shared<mock_component>();

    container_.set_size({2, 2});

    container_.add_component(component);

    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface{canvas};

    EXPECT_CALL(*component, do_get_position())
        .WillOnce(Return(terminalpp::point(1, 0)));
    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(terminalpp::extent(1, 2)));

    container_.draw(surface, terminalpp::rectangle({0, 0}, {1, 2}));
}

TEST_F(a_container, offsets_canvas_before_drawing_offset_components)
{
    auto component = std::make_shared<mock_component>();

    container_.set_size({2, 2});

    container_.add_component(component);

    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface{canvas};

    EXPECT_CALL(*component, do_get_position())
        .WillOnce(Return(terminalpp::point(1, 0)));
    EXPECT_CALL(*component, do_get_size())
        .WillOnce(Return(terminalpp::extent(1, 2)));

    EXPECT_CALL(*component, do_draw(_, terminalpp::rectangle({0, 0}, {1, 2})));

    container_.draw(surface, terminalpp::rectangle({0, 0}, {2, 2}));
}

TEST_F(a_container, draws_many_components_when_drawing)
{
    auto component_tl = std::make_shared<mock_component>();
    auto component_tr = std::make_shared<mock_component>();
    auto component_bl = std::make_shared<mock_component>();
    auto component_br = std::make_shared<mock_component>();

    container_.set_size({4, 4});
    container_.add_component(component_tl);
    container_.add_component(component_tr);
    container_.add_component(component_bl);
    container_.add_component(component_br);

    terminalpp::canvas canvas({2, 2});
    munin::render_surface surface{canvas};

    EXPECT_CALL(*component_tl, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 0)));
    EXPECT_CALL(*component_tl, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));
    EXPECT_CALL(
        *component_tl, do_draw(_, terminalpp::rectangle({1, 1}, {1, 1})));

    EXPECT_CALL(*component_tr, do_get_position())
        .WillOnce(Return(terminalpp::point(2, 0)));
    EXPECT_CALL(*component_tr, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));
    EXPECT_CALL(
        *component_tr, do_draw(_, terminalpp::rectangle({0, 1}, {1, 1})));

    EXPECT_CALL(*component_bl, do_get_position())
        .WillOnce(Return(terminalpp::point(0, 2)));
    EXPECT_CALL(*component_bl, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));
    EXPECT_CALL(
        *component_bl, do_draw(_, terminalpp::rectangle({1, 0}, {1, 1})));

    EXPECT_CALL(*component_br, do_get_position())
        .WillOnce(Return(terminalpp::point(2, 2)));
    EXPECT_CALL(*component_br, do_get_size())
        .WillOnce(Return(terminalpp::extent(2, 2)));
    EXPECT_CALL(
        *component_br, do_draw(_, terminalpp::rectangle({0, 0}, {1, 1})));

    container_.draw(surface, terminalpp::rectangle({1, 1}, {2, 2}));
}
