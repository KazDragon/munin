#include "text_area_test.hpp"

TEST_F(a_text_area, is_a_component)
{
    std::shared_ptr<munin::component> comp = munin::make_text_area();
}
