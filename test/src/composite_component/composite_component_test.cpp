#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/composite_component.hpp>
#include <munin/grid_layout.hpp>

using testing::_;
using testing::Return;

class composite_mock : public munin::composite_component
{
public:
    composite_mock()
    {
        set_layout(munin::make_grid_layout({1, 1}));
        add_component(inner_component);
    }

    std::shared_ptr<mock_component> inner_component{make_mock_component()};
};

class a_composite_component : public testing::Test
{
protected:
    std::shared_ptr<composite_mock> composite_{
        std::make_shared<composite_mock>()};
};

TEST_F(a_composite_component, returns_the_inner_components_cursor_state)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(*composite_->inner_component, do_get_cursor_state())
            .WillOnce(Return(false));
        EXPECT_CALL(*composite_->inner_component, do_get_cursor_state())
            .WillOnce(Return(true));
    }

    // Note: In a container, only components with focus get asked their
    // cursor state.
    composite_->set_focus();
    ON_CALL(*composite_->inner_component, do_has_focus)
        .WillByDefault(Return(true));

    ASSERT_FALSE(composite_->get_cursor_state());
    ASSERT_TRUE(composite_->get_cursor_state());
}

TEST_F(a_composite_component, returns_the_inner_components_cursor_position)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(*composite_->inner_component, do_get_cursor_position())
            .WillOnce(Return(terminalpp::point{1, 1}));
        EXPECT_CALL(*composite_->inner_component, do_get_cursor_position())
            .WillOnce(Return(terminalpp::point{2, 2}));
    }

    // Note: In a container, only components with focus get asked their
    // cursor state.
    composite_->set_focus();
    ON_CALL(*composite_->inner_component, do_has_focus)
        .WillByDefault(Return(true));

    ASSERT_EQ(terminalpp::point(1, 1), composite_->get_cursor_position());
    ASSERT_EQ(terminalpp::point(2, 2), composite_->get_cursor_position());
}

TEST_F(a_composite_component, sets_the_cursor_position_of_its_inner_component)
{
    static constexpr terminalpp::point pos{2, 7};
    EXPECT_CALL(*composite_->inner_component, do_set_cursor_position(pos));

    composite_->set_focus();
    ON_CALL(*composite_->inner_component, do_has_focus)
        .WillByDefault(Return(true));

    composite_->set_cursor_position(pos);
}

TEST_F(a_composite_component, forwards_events_to_its_inner_component)
{
    static std::string const value = "test";
    std::string received_value;

    EXPECT_CALL(*composite_->inner_component, do_event(_))
        .WillOnce([&received_value](boost::any const &ev) {
            received_value = boost::any_cast<std::string>(ev);
        });

    composite_->set_focus();
    ON_CALL(*composite_->inner_component, do_has_focus)
        .WillByDefault(Return(true));

    composite_->event(value);
    ASSERT_EQ(value, received_value);
}
