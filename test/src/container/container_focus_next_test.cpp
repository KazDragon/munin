#include "container_test.hpp"

using testing::InSequence;
using testing::Return;

TEST_F(
    a_container_with_one_component,
    calls_focus_next_on_subcomponent_on_focus_next)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  {
    InSequence s1;
    EXPECT_CALL(*component, do_focus_next())
        .WillOnce(std::ref(component->on_focus_set));
    EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(true));
  }

  container.focus_next();

  ASSERT_TRUE(container.has_focus());
  ASSERT_EQ(1, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_one_component,
    that_refuses_focus_refuses_focus_on_focus_next)
{
  EXPECT_CALL(*component, do_has_focus()).WillRepeatedly(Return(false));

  {
    InSequence s1;
    EXPECT_CALL(*component, do_focus_next());
    EXPECT_CALL(*component, do_has_focus()).WillOnce(Return(false));
  }

  container.focus_next();

  ASSERT_FALSE(container.has_focus());
  ASSERT_EQ(0, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_two_components,
    skips_components_that_refuse_focus_next_on_focus_next)
{
  EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));
  EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(false));

  {
    InSequence s1;
    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus()).WillRepeatedly(Return(false));

    EXPECT_CALL(*component1, do_focus_next())
        .WillOnce(std::ref(component1->on_focus_set));
    EXPECT_CALL(*component1, do_has_focus()).WillRepeatedly(Return(true));
  }

  container.focus_next();

  ASSERT_TRUE(container.has_focus());
  ASSERT_EQ(1, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    does_not_yield_focus_on_next_focus_when_the_component_does_not_yield_focus)
{
  {
    InSequence s1;
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(true));
    EXPECT_CALL(*component0, do_focus_next());
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(true));
  }

  container.focus_next();

  ASSERT_TRUE(container.has_focus());
  ASSERT_EQ(0, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    calls_next_focus_on_next_subcomponent_when_the_focused_component_yields_focus_on_next_focus)
{
  {
    InSequence s1;
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(true));
    EXPECT_CALL(*component0, do_focus_next())
        .WillOnce(std::ref(component0->on_focus_lost));
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(false));

    EXPECT_CALL(*component1, do_focus_next())
        .WillOnce(std::ref(component1->on_focus_set));
    EXPECT_CALL(*component1, do_has_focus()).WillOnce(Return(true));
  }

  container.focus_next();

  ASSERT_TRUE(container.has_focus());
  ASSERT_EQ(0, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_three_components_where_the_first_has_focus,
    skips_components_that_refuse_focus_when_the_focused_component_yields_focus_on_next_focus)
{
  {
    InSequence s1;
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(true));
    EXPECT_CALL(*component0, do_focus_next())
        .WillOnce(std::ref(component0->on_focus_lost));
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(false));

    EXPECT_CALL(*component1, do_focus_next());
    EXPECT_CALL(*component1, do_has_focus()).WillOnce(Return(false));

    EXPECT_CALL(*component2, do_focus_next())
        .WillOnce(std::ref(component2->on_focus_set));
    EXPECT_CALL(*component2, do_has_focus()).WillOnce(Return(true));
  }

  container.focus_next();

  ASSERT_TRUE(container.has_focus());
  ASSERT_EQ(0, focus_set_count);
  ASSERT_EQ(0, focus_lost_count);
}

TEST_F(
    a_container_with_two_components_where_the_first_has_focus,
    loses_focus_on_next_focus_if_no_component_accepts_next_focus)
{
  {
    InSequence s1;
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(true));
    EXPECT_CALL(*component0, do_focus_next())
        .WillOnce(std::ref(component0->on_focus_lost));
    EXPECT_CALL(*component0, do_has_focus()).WillOnce(Return(false));

    EXPECT_CALL(*component1, do_focus_next());
    EXPECT_CALL(*component1, do_has_focus()).WillOnce(Return(false));
  }

  container.focus_next();

  ASSERT_FALSE(container.has_focus());
  ASSERT_EQ(0, focus_set_count);
  ASSERT_EQ(1, focus_lost_count);
}
