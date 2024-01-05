#include "container_test.hpp"
#include "mock/layout.hpp"

using testing::_;
using testing::Return;

TEST_F(a_new_container, reports_attributes_as_json)
{
  munin::component &comp = container;

  nlohmann::json json = comp.to_json();
  ASSERT_EQ("container", json["type"]);
  ASSERT_EQ(0, json["position"]["x"]);
  ASSERT_EQ(0, json["position"]["y"]);
  ASSERT_EQ(0, json["size"]["height"]);
  ASSERT_EQ(0, json["size"]["width"]);
  ASSERT_EQ(0, json["preferred_size"]["width"]);
  ASSERT_EQ(0, json["preferred_size"]["height"]);
  ASSERT_EQ(false, json["has_focus"]);
  ASSERT_EQ(false, json["cursor_state"]);
  ASSERT_EQ(0, json["cursor_position"]["x"]);
  ASSERT_EQ(0, json["cursor_position"]["y"]);
  ASSERT_EQ("null_layout", json["layout"]["type"]);
}

TEST_F(a_container_with_one_component, reports_attributes_as_json)
{
  static constexpr terminalpp::extent const layout_size{3, 5};
  auto lyt = make_mock_layout();

  EXPECT_CALL(*lyt, do_get_preferred_size(_, _))
      .WillRepeatedly(Return(layout_size));
  ON_CALL(*lyt, do_layout(_, _, _)).WillByDefault(Return());
  ON_CALL(*lyt, do_to_json())
      .WillByDefault(
          [] {
            return nlohmann::json{{"type", "mock_layout"}};
          });

  container.set_layout(std::move(lyt));

  munin::component &comp = container;

  nlohmann::json component_json;
  component_json["test"] = "succeeded";

  EXPECT_CALL(*component, do_to_json()).WillRepeatedly(Return(component_json));

  nlohmann::json json = comp.to_json();
  ASSERT_EQ("container", json["type"]);
  ASSERT_EQ(0, json["position"]["x"]);
  ASSERT_EQ(0, json["position"]["y"]);
  ASSERT_EQ(0, json["size"]["height"]);
  ASSERT_EQ(0, json["size"]["width"]);
  ASSERT_EQ(layout_size.width_, json["preferred_size"]["width"]);
  ASSERT_EQ(layout_size.height_, json["preferred_size"]["height"]);
  ASSERT_EQ(false, json["has_focus"]);
  ASSERT_EQ(false, json["cursor_state"]);
  ASSERT_EQ(0, json["cursor_position"]["x"]);
  ASSERT_EQ(0, json["cursor_position"]["y"]);
  ASSERT_EQ("mock_layout", json["layout"]["type"]);

  nlohmann::json subcomponents = json["subcomponents"];
  ASSERT_EQ(1, subcomponents.size());
  ASSERT_EQ(component_json, subcomponents[0]);
}

TEST_F(a_container_with_two_components, reports_attributes_as_json)
{
  static constexpr terminalpp::extent const layout_size{3, 5};
  auto lyt = make_mock_layout();

  EXPECT_CALL(*lyt, do_get_preferred_size(_, _))
      .WillRepeatedly(Return(layout_size));
  ON_CALL(*lyt, do_layout(_, _, _)).WillByDefault(Return());

  container.set_layout(std::move(lyt));

  munin::component &comp = container;

  nlohmann::json component0_json;
  component0_json["test"] = "succeeded";

  EXPECT_CALL(*component0, do_to_json())
      .WillRepeatedly(Return(component0_json));

  nlohmann::json component1_json;
  component1_json["more"] = "testing";
  component1_json["even"] = "worked";

  EXPECT_CALL(*component1, do_to_json())
      .WillRepeatedly(Return(component1_json));

  nlohmann::json json = comp.to_json();
  ASSERT_EQ("container", json["type"]);
  ASSERT_EQ(0, json["position"]["x"]);
  ASSERT_EQ(0, json["position"]["y"]);
  ASSERT_EQ(0, json["size"]["height"]);
  ASSERT_EQ(0, json["size"]["width"]);
  ASSERT_EQ(layout_size.width_, json["preferred_size"]["width"]);
  ASSERT_EQ(layout_size.height_, json["preferred_size"]["height"]);
  ASSERT_EQ(false, json["has_focus"]);
  ASSERT_EQ(false, json["cursor_state"]);
  ASSERT_EQ(0, json["cursor_position"]["x"]);
  ASSERT_EQ(0, json["cursor_position"]["y"]);

  nlohmann::json subcomponents = json["subcomponents"];
  ASSERT_EQ(2, subcomponents.size());
  ASSERT_EQ(component0_json, subcomponents[0]);
  ASSERT_EQ(component1_json, subcomponents[1]);
}

TEST_F(
    a_container_with_one_component_that_has_focus, reports_attributes_as_json)
{
  EXPECT_CALL(*component, do_get_position())
      .WillRepeatedly(Return(terminalpp::point(1, 2)));
  EXPECT_CALL(*component, do_get_cursor_position())
      .WillRepeatedly(Return(terminalpp::point(3, 4)));
  EXPECT_CALL(*component, do_get_cursor_state()).WillRepeatedly(Return(true));
  EXPECT_CALL(*component, do_to_json())
      .WillRepeatedly(Return(nlohmann::json{}));
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));

  nlohmann::json json = container.to_json();
  ASSERT_EQ("container", json["type"]);
  ASSERT_EQ(0, json["position"]["x"]);
  ASSERT_EQ(0, json["position"]["y"]);
  ASSERT_EQ(0, json["size"]["height"]);
  ASSERT_EQ(0, json["size"]["width"]);
  ASSERT_EQ(0, json["preferred_size"]["width"]);
  ASSERT_EQ(0, json["preferred_size"]["height"]);
  ASSERT_EQ(true, json["has_focus"]);
  ASSERT_EQ(true, json["cursor_state"]);
  ASSERT_EQ(4, json["cursor_position"]["x"]);
  ASSERT_EQ(6, json["cursor_position"]["y"]);
  ASSERT_EQ("null_layout", json["layout"]["type"]);
}
