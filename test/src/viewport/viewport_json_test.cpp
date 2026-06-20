#include "../basic_component/fake_basic_component.hpp"

#include <gtest/gtest.h>
#include <munin/viewport.hpp>

class a_viewport_json_snapshot : public testing::Test
{
protected:
    [[nodiscard]] nlohmann::json current_json() const
    {
        return viewport_->to_json();
    }

    std::shared_ptr<munin::component> tracked_component_{
        std::make_shared<fake_basic_component>()};
    std::shared_ptr<munin::viewport> viewport_{
        munin::make_viewport(tracked_component_)};
};

TEST_F(a_viewport_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("viewport", json.at("type"));
}
