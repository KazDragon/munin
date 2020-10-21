#include <munin/list.hpp>
#include <gtest/gtest.h>

TEST(a_list, is_a_component)
{
    std::shared_ptr<munin::list> list = munin::make_list();
    std::shared_ptr<munin::component> component = list;
}
