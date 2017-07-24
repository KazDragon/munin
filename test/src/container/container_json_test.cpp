#include "container_test.hpp"
#include "mock/layout.hpp"

using testing::Return;
using testing::_;

TEST_F(a_new_container, reports_attributes_as_json)
{
    munin::component &comp = container;

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("container", json["type"]);
    ASSERT_EQ(0,           json["position"]["x"]);
    ASSERT_EQ(0,           json["position"]["y"]);
    ASSERT_EQ(0,           json["size"]["height"]);
    ASSERT_EQ(0,           json["size"]["width"]);
    ASSERT_EQ(0,           json["preferred_size"]["width"]);
    ASSERT_EQ(0,           json["preferred_size"]["height"]);
    ASSERT_EQ(false,       json["has_focus"]);
    ASSERT_EQ(false,       json["cursor_state"]);
    ASSERT_EQ(0,           json["cursor_position"]["x"]);
    ASSERT_EQ(0,           json["cursor_position"]["y"]);
}

TEST_F(a_container_with_one_component, reports_attributes_as_json)
{
    static constexpr terminalpp::extent const layout_size{3, 5};
    auto lyt = std::unique_ptr<mock_layout>(new mock_layout);

    EXPECT_CALL(*lyt, do_get_preferred_size(_, _))
        .WillRepeatedly(Return(layout_size));
    ON_CALL(*lyt, do_layout(_, _, _))
        .WillByDefault(Return());

    container.set_layout(std::move(lyt));

    munin::component &comp = container;

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("container",        json["type"]);
    ASSERT_EQ(0,                  json["position"]["x"]);
    ASSERT_EQ(0,                  json["position"]["y"]);
    ASSERT_EQ(0,                  json["size"]["height"]);
    ASSERT_EQ(0,                  json["size"]["width"]);
    ASSERT_EQ(layout_size.width,  json["preferred_size"]["width"]);
    ASSERT_EQ(layout_size.height, json["preferred_size"]["height"]);
    ASSERT_EQ(false,              json["has_focus"]);
    ASSERT_EQ(false,              json["cursor_state"]);
    ASSERT_EQ(0,                  json["cursor_position"]["x"]);
    ASSERT_EQ(0,                  json["cursor_position"]["y"]);

}
