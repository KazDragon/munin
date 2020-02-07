#include "mock/component.hpp"
#include <munin/viewport.hpp>

class a_viewport_with_mock_tracked_component
{
protected:
    std::shared_ptr<mock_component> tracked_component_ = make_mock_component();
    std::shared_ptr<munin::viewport> viewport_ = munin::make_viewport(tracked_component_);
};

template <typename TestData>
class viewport_mock_test_with_data
  : public a_viewport_with_mock_tracked_component,
    public testing::TestWithParam<TestData>
{
protected:
    viewport_mock_test_with_data()
    {
        using testing::Invoke;
        using testing::Return;
        using testing::_;
        
        static auto constexpr tracked_component_preferred_size = terminalpp::extent{6, 6};
        static auto constexpr viewport_size = terminalpp::extent{3, 3};

        // Set the preferred size of the tracked component.  It is tested elsewhere that
        // a viewport allows the tracked component its preferred size.
        ON_CALL(*tracked_component_, do_get_preferred_size())
            .WillByDefault(Return(tracked_component_preferred_size));
        tracked_component_->on_preferred_size_changed();

        // Mock the cursor position of the tracked component so that it does what it
        // is told and announces it to the viewport.
        ON_CALL(*tracked_component_, do_set_cursor_position(_))
            .WillByDefault(Invoke([this](auto const &pos) { 
                tracked_cursor_position_ = pos;
                tracked_component_->on_cursor_position_changed();
            }));
        ON_CALL(*tracked_component_, do_get_cursor_position())
            .WillByDefault(Invoke([this] { return tracked_cursor_position_; }));

        viewport_->set_size(viewport_size);
    }

private:
    terminalpp::point tracked_cursor_position_;
};
