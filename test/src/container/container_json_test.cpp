#include "container_test.hpp"
#include "mock/layout.hpp"

using testing::_;
using testing::Return;

namespace {

void assert_json_eq(
    terminalpp::point const &expected, nlohmann::json const &json)
{
    ASSERT_EQ(expected.x_, json["x"]);
    ASSERT_EQ(expected.y_, json["y"]);
}

void assert_json_eq(
    terminalpp::extent const &expected, nlohmann::json const &json)
{
    ASSERT_EQ(expected.width_, json["width"]);
    ASSERT_EQ(expected.height_, json["height"]);
}

void allow_layout(mock_layout &lyt)
{
    ON_CALL(lyt, do_layout(_, _, _)).WillByDefault(Return());
}

std::unique_ptr<mock_layout> make_sized_layout(
    terminalpp::extent const &preferred_size)
{
    auto lyt = make_mock_layout();
    allow_layout(*lyt);
    EXPECT_CALL(*lyt, do_get_preferred_size(_, _))
        .WillRepeatedly(Return(preferred_size));
    return lyt;
}

std::unique_ptr<mock_layout> make_json_layout(
    terminalpp::extent const &preferred_size, std::string type)
{
    auto lyt = make_sized_layout(preferred_size);
    EXPECT_CALL(*lyt, do_to_json()).WillRepeatedly([type = std::move(type)] {
        return nlohmann::json{
            {"type", type},
        };
    });
    return lyt;
}

}  // namespace

TEST_F(a_new_container, reports_attributes_as_json)
{
    munin::component &comp = container_;

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("container", json["type"]);
    assert_json_eq(terminalpp::point{}, json["position"]);
    assert_json_eq(terminalpp::extent{}, json["size"]);
    assert_json_eq(terminalpp::extent{}, json["preferred_size"]);
    ASSERT_EQ(false, json["has_focus"]);
    ASSERT_EQ(false, json["cursor_state"]);
    assert_json_eq(terminalpp::point{}, json["cursor_position"]);
    ASSERT_EQ("null_layout", json["layout"]["type"]);
}

TEST_F(a_new_container, reports_its_automation_id_as_json)
{
    munin::component &comp = container_;

    comp.set_id("inventory_panel");

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("inventory_panel", json["id"]);
}

TEST_F(a_new_container, reports_its_position_as_json)
{
    munin::component &comp = container_;
    terminalpp::point const position{6, 9};

    comp.set_position(position);

    nlohmann::json json = comp.to_json();
    assert_json_eq(position, json["position"]);
}

TEST_F(a_new_container, reports_its_size_as_json)
{
    munin::component &comp = container_;
    terminalpp::extent const size{17, 23};

    comp.set_size(size);

    nlohmann::json json = comp.to_json();
    assert_json_eq(size, json["size"]);
}

TEST_F(a_new_container, reports_the_current_layout_as_json)
{
    auto first_layout = make_mock_layout();
    auto second_layout =
        make_json_layout(terminalpp::extent{}, "replacement_layout");

    allow_layout(*first_layout);

    container_.set_layout(std::move(first_layout));
    container_.set_layout(std::move(second_layout));

    nlohmann::json json = container_.to_json();
    ASSERT_EQ("replacement_layout", json["layout"]["type"]);
}

TEST_F(a_container_with_one_component, reports_attributes_as_json)
{
    static constexpr terminalpp::extent const layout_size{3, 5};
    auto lyt = make_json_layout(layout_size, "mock_layout");

    container_.set_layout(std::move(lyt));

    munin::component &comp = container_;

    nlohmann::json component_json;
    component_json["test"] = "succeeded";

    EXPECT_CALL(*component_, do_to_json())
        .WillRepeatedly(Return(component_json));

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("container", json["type"]);
    assert_json_eq(terminalpp::point{}, json["position"]);
    assert_json_eq(terminalpp::extent{}, json["size"]);
    assert_json_eq(layout_size, json["preferred_size"]);
    ASSERT_EQ(false, json["has_focus"]);
    ASSERT_EQ(false, json["cursor_state"]);
    assert_json_eq(terminalpp::point{}, json["cursor_position"]);
    ASSERT_EQ("mock_layout", json["layout"]["type"]);

    nlohmann::json subcomponents = json["subcomponents"];
    ASSERT_EQ(1, subcomponents.size());
    ASSERT_EQ(component_json, subcomponents[0]);
}

TEST_F(a_container_with_two_components, reports_attributes_as_json)
{
    static constexpr terminalpp::extent const layout_size{3, 5};
    auto lyt = make_sized_layout(layout_size);

    container_.set_layout(std::move(lyt));

    munin::component &comp = container_;

    nlohmann::json component0_json;
    component0_json["test"] = "succeeded";

    EXPECT_CALL(*component0_, do_to_json())
        .WillRepeatedly(Return(component0_json));

    nlohmann::json component1_json;
    component1_json["more"] = "testing";
    component1_json["even"] = "worked";

    EXPECT_CALL(*component1_, do_to_json())
        .WillRepeatedly(Return(component1_json));

    nlohmann::json json = comp.to_json();
    ASSERT_EQ("container", json["type"]);
    assert_json_eq(terminalpp::point{}, json["position"]);
    assert_json_eq(terminalpp::extent{}, json["size"]);
    assert_json_eq(layout_size, json["preferred_size"]);
    ASSERT_EQ(false, json["has_focus"]);
    ASSERT_EQ(false, json["cursor_state"]);
    assert_json_eq(terminalpp::point{}, json["cursor_position"]);

    nlohmann::json subcomponents = json["subcomponents"];
    ASSERT_EQ(2, subcomponents.size());
    ASSERT_EQ(component0_json, subcomponents[0]);
    ASSERT_EQ(component1_json, subcomponents[1]);
}

TEST_F(
    a_container_with_one_component_that_has_focus, reports_attributes_as_json)
{
    EXPECT_CALL(*component_, do_get_position())
        .WillRepeatedly(Return(terminalpp::point(1, 2)));
    EXPECT_CALL(*component_, do_get_cursor_position())
        .WillRepeatedly(Return(terminalpp::point(3, 4)));
    EXPECT_CALL(*component_, do_get_cursor_state())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*component_, do_to_json())
        .WillRepeatedly(Return(nlohmann::json{}));
    EXPECT_CALL(*component_, do_has_focus()).WillRepeatedly(Return(true));

    nlohmann::json json = container_.to_json();
    ASSERT_EQ("container", json["type"]);
    assert_json_eq(terminalpp::point{}, json["position"]);
    assert_json_eq(terminalpp::extent{}, json["size"]);
    assert_json_eq(terminalpp::extent{}, json["preferred_size"]);
    ASSERT_EQ(true, json["has_focus"]);
    ASSERT_EQ(true, json["cursor_state"]);
    assert_json_eq(terminalpp::point(4, 6), json["cursor_position"]);
    ASSERT_EQ("null_layout", json["layout"]["type"]);
}
