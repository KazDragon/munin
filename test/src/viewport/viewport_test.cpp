#include "fill_canvas.hpp"
#include "mock/component.hpp"
#include <munin/edit.hpp>
#include <munin/render_surface.hpp>
#include <munin/viewport.hpp>
#include <terminalpp/canvas.hpp>
#include <gtest/gtest.h>

using testing::Return;
using testing::Values;

namespace {
    
class a_new_viewport : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_{munin::make_edit()};
    std::shared_ptr<munin::viewport> viewport_{munin::make_viewport(edit_)};
};

}

TEST_F(a_new_viewport, is_a_component)
{
    std::shared_ptr<munin::component> comp = viewport_;
}

TEST_F(a_new_viewport, of_zero_size_draws_nothing)
{
    terminalpp::canvas cvs{{4, 3}};
    fill_canvas(cvs, 'x');
    
    viewport_->set_position({1, 1});
    viewport_->set_size({0, 0});
    
    munin::render_surface surface{cvs};
    surface.offset_by({1, 1});
    viewport_->draw(surface, {{}, viewport_->get_size()});
    
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][0]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][1]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[0][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[1][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[2][2]);
    ASSERT_EQ(terminalpp::element{'x'}, cvs[3][2]);
}

namespace {

class viewport_preferred_size_test 
  : public testing::TestWithParam<terminalpp::extent>
{
protected:
    std::shared_ptr<mock_component> tracked_component_ = make_mock_component();
    std::shared_ptr<munin::viewport> viewport_ = munin::make_viewport(tracked_component_);
};

}

TEST_P(viewport_preferred_size_test, the_preferred_size_of_a_viewport_is_the_same_as_the_preferred_size_of_the_tracked_component)
{
    auto const preferred_size = GetParam();
    
    bool preferred_size_changed = false;
    viewport_->on_preferred_size_changed.connect(
        [&]() { preferred_size_changed = true; });
        
    ON_CALL(*tracked_component_, do_get_preferred_size())
        .WillByDefault(Return(preferred_size));
        
    tracked_component_->on_preferred_size_changed();
    
    ASSERT_EQ(preferred_size, viewport_->get_preferred_size());
    ASSERT_TRUE(preferred_size_changed);
}

INSTANTIATE_TEST_CASE_P(
    viewport_size,
    viewport_preferred_size_test,
    Values
    (
        terminalpp::extent(0, 0),
        terminalpp::extent(1, 1),
        terminalpp::extent(4, 5)
    )
);

