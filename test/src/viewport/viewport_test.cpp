#include <munin/edit.hpp>
#include <munin/viewport.hpp>
#include <gtest/gtest.h>

class a_new_viewport : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_{munin::make_edit()};
    std::shared_ptr<munin::viewport> viewport_{munin::make_viewport(edit_)};
};

TEST_F(a_new_viewport, is_a_component)
{
    std::shared_ptr<munin::component> comp = viewport_;
}