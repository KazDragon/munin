#include <munin/text_area.hpp>
#include <gtest/gtest.h>

class a_text_area : public testing::Test
{
protected:
    munin::text_area text_area_;
};

TEST_F(a_text_area, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_text_area();
}
