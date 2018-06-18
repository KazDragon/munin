#include "mock/component.hpp"
#include <munin/framed_component.hpp>
#include <gtest/gtest.h>

using testing::_;
using testing::Invoke;

TEST(clicking_the_mouse_on_the_inner_component, sends_the_mouse_event_to_the_inner_component)
{
    auto mock_frame = make_mock_component();
    auto mock_inner = make_mock_component();
    
    // Fake out the position and size functions for the frame/inner component
    terminalpp::point mock_frame_position;
    terminalpp::point mock_inner_position;
    terminalpp::extent mock_frame_size;
    terminalpp::extent mock_inner_size;
    
    ON_CALL(*mock_frame, do_set_position(_))
        .WillByDefault(Invoke([&mock_frame_position](auto const &position)
        {
            mock_frame_position = position;
        }));
    ON_CALL(*mock_frame, do_get_position())
        .WillByDefault(Invoke([&mock_frame_position]{return mock_frame_position;}));
    ON_CALL(*mock_frame, do_set_size(_))
        .WillByDefault(Invoke([&mock_frame_size](auto const &size)
        {
            mock_frame_size = size;
        }));
    ON_CALL(*mock_frame, do_get_size())
        .WillByDefault(Invoke([&mock_frame_size]{return mock_frame_size;}));
    
    ON_CALL(*mock_inner, do_set_position(_))
        .WillByDefault(Invoke([&mock_inner_position](auto const &position)
        {
            mock_inner_position = position;
        }));
    ON_CALL(*mock_inner, do_get_position())
        .WillByDefault(Invoke([&mock_inner_position]{return mock_inner_position;}));
    ON_CALL(*mock_inner, do_set_size(_))
        .WillByDefault(Invoke([&mock_inner_size](auto const &size)
        {
            mock_inner_size = size;
        }));
    ON_CALL(*mock_inner, do_get_size())
        .WillByDefault(Invoke([&mock_inner_size]{return mock_inner_size;}));
    
    std::shared_ptr<munin::framed_component> comp = munin::make_framed_component(
        mock_frame,
        mock_inner
    );
    
    comp->set_position({0, 0});
    comp->set_size({4, 4});

    auto received_mouse_report = terminalpp::ansi::mouse::report{};

    EXPECT_CALL(*mock_inner, do_event(_))
        .WillOnce(Invoke([&received_mouse_report](auto ev)
        {
            auto *mouse_report = 
                boost::any_cast<terminalpp::ansi::mouse::report>(&ev);
                
            if (mouse_report)
            {
                received_mouse_report = *mouse_report;
            }
        }));

    auto const sent_mouse_report = terminalpp::ansi::mouse::report{
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
        2, 2
    };
    
    comp->event(sent_mouse_report);

    auto const expected_mouse_report = terminalpp::ansi::mouse::report{
        terminalpp::ansi::mouse::report::LEFT_BUTTON_DOWN,
        1, 1
    };

    ASSERT_EQ(expected_mouse_report, received_mouse_report);
}