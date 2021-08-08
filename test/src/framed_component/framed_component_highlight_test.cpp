#include "mock/component.hpp"
#include "mock/frame.hpp"
#include <munin/framed_component.hpp>
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <gtest/gtest.h>

using testing::Return;
using testing::_;

namespace {

constexpr terminalpp::glyph frame_glyph = '!';

constexpr terminalpp::attribute lowlight_attribute {
    terminalpp::greyscale_colour{ 5 }
};

constexpr terminalpp::attribute highlight_attribute { 
    terminalpp::high_colour{4, 5, 1}
};

class mock_frame_with_focus_attribute : public mock_frame
{
public:
    terminalpp::attribute get_frame_focus_attribute() const
    {
        return get_focus_attribute();
    }
};

std::shared_ptr<mock_frame_with_focus_attribute>
    make_mock_frame_with_focus_attribute()
{
    return std::make_shared<
        testing::NiceMock<mock_frame_with_focus_attribute>>();
}

class a_framed_component_with_an_inner_component
  : public testing::Test
{
protected:
    a_framed_component_with_an_inner_component()
    {
        mock_frame_->set_lowlight_attribute(lowlight_attribute);
        mock_frame_->set_highlight_attribute(highlight_attribute);

        ON_CALL(*mock_frame_, north_border_height())
            .WillByDefault(Return(1));
        ON_CALL(*mock_frame_, south_border_height())
            .WillByDefault(Return(1));
        ON_CALL(*mock_frame_, west_border_width())
            .WillByDefault(Return(1));
        ON_CALL(*mock_frame_, east_border_width())
            .WillByDefault(Return(1));

        ON_CALL(*mock_frame_, do_get_size())
            .WillByDefault(Return(terminalpp::extent{3, 3}));

        ON_CALL(*mock_frame_, do_draw(_, _))
            .WillByDefault(
                [this](auto &surface, auto const &region)
                {
                    auto const &focus_element = terminalpp::element{
                        frame_glyph,
                        mock_frame_->get_frame_focus_attribute()
                    };

                    terminalpp::for_each_in_region(
                        surface,
                        region,
                        [&focus_element](terminalpp::element &elem, 
                            terminalpp::coordinate_type column, 
                            terminalpp::coordinate_type row)
                        {
                            elem = focus_element;
                        });
                });

        framed_component_->set_size({3, 3});
    }

    std::shared_ptr<mock_frame_with_focus_attribute> mock_frame_{
        make_mock_frame_with_focus_attribute()
    };
    std::shared_ptr<mock_component> mock_inner_{make_mock_component()};
    std::shared_ptr<munin::framed_component> framed_component_{
        munin::make_framed_component(mock_frame_, mock_inner_)
    };

    terminalpp::canvas canvas_{{3, 3}};
    munin::render_surface surface_{canvas_};
};

}

TEST_F(a_framed_component_with_an_inner_component, has_the_lowlight_attribute_when_the_inner_component_does_not_have_focus)
{
    framed_component_->draw(surface_, {{}, framed_component_->get_size()});

    auto const expected_element = terminalpp::element {
        frame_glyph,
        lowlight_attribute
    };

    ASSERT_EQ(expected_element, canvas_[0][0]);
}

TEST_F(a_framed_component_with_an_inner_component, has_the_highlight_attribute_when_the_inner_component_has_focus)
{
    ON_CALL(*mock_inner_, do_has_focus())
        .WillByDefault(Return(true));

    framed_component_->draw(surface_, {{}, framed_component_->get_size()});

    auto const expected_element = terminalpp::element {
        frame_glyph,
        highlight_attribute
    };

    ASSERT_EQ(expected_element, canvas_[0][0]);
}