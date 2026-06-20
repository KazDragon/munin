#include "mock/component.hpp"

#include <gtest/gtest.h>
#include <munin/viewport.hpp>

class a_viewport_json_snapshot : public testing::Test
{
protected:
    a_viewport_json_snapshot()
    {
        using testing::_;
        using testing::ReturnPointee;
        using testing::SaveArg;

        ON_CALL(*tracked_component_, do_get_size())
            .WillByDefault(ReturnPointee(&tracked_component_size_));
        ON_CALL(*tracked_component_, do_set_size(_))
            .WillByDefault(SaveArg<0>(&tracked_component_size_));
        ON_CALL(*tracked_component_, do_get_preferred_size())
            .WillByDefault(ReturnPointee(&tracked_component_preferred_size_));
        ON_CALL(*tracked_component_, do_get_cursor_position())
            .WillByDefault(ReturnPointee(&tracked_cursor_position_));
    }

    [[nodiscard]] nlohmann::json current_json() const
    {
        return viewport_->to_json();
    }

    void move_tracked_cursor_to(terminalpp::point const &position)
    {
        tracked_cursor_position_ = position;
        tracked_component_->on_cursor_position_changed();
    }

    std::shared_ptr<mock_component> tracked_component_{make_mock_component()};
    std::shared_ptr<munin::viewport> viewport_{
        munin::make_viewport(tracked_component_)};

private:
    terminalpp::extent tracked_component_preferred_size_{6, 6};
    terminalpp::extent tracked_component_size_;
    terminalpp::point tracked_cursor_position_;
};

TEST_F(a_viewport_json_snapshot, reports_its_type)
{
    auto const json = current_json();

    ASSERT_EQ("viewport", json.at("type"));
}

TEST_F(a_viewport_json_snapshot, reports_its_anchor_bounds)
{
    viewport_->set_size({3, 3});
    move_tracked_cursor_to({5, 4});

    auto const json = current_json();

    ASSERT_EQ(3, json.at("anchor_bounds").at("origin").at("x"));
    ASSERT_EQ(2, json.at("anchor_bounds").at("origin").at("y"));
    ASSERT_EQ(3, json.at("anchor_bounds").at("size").at("width"));
    ASSERT_EQ(3, json.at("anchor_bounds").at("size").at("height"));
}
