#include <munin/edit.hpp>
#include <gtest/gtest.h>

class a_new_edit : public testing::Test
{
protected:
    std::shared_ptr<munin::edit> edit_ = munin::make_edit();
};

TEST_F(a_new_edit, is_a_component)
{
    std::shared_ptr<munin::component> edit = edit_;
}