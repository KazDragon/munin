#include "mock/component.hpp"
#include <munin/compass_layout.hpp>
#include <munin/rectangle.hpp>
#include <gtest/gtest.h>

using testing::Return;
using testing::ValuesIn;

TEST(compass_layout_test, reports_attributes_as_json)
{
    munin::compass_layout cl;
    munin::layout &lyt = cl;

    nlohmann::json json = lyt.to_json();

    ASSERT_EQ("compass_layout", json["type"]);
}

using compass_layout_component_data = std::tuple<
    terminalpp::extent, // preferred size
    boost::any,         // layout hint
    munin::rectangle    // expected size
>;

using compass_layout_test_data = std::tuple<
    std::vector<compass_layout_component_data>, // component data
    terminalpp::extent                          // container size
>;

class compass_layouts
  : public testing::TestWithParam<compass_layout_test_data>
{
};

TEST_P(compass_layouts, place_components_at_these_positions)
{
    auto const &param = GetParam();
    auto const &component_data = std::get<0>(param);
    auto const &container_size = std::get<1>(param);

    std::vector<std::shared_ptr<munin::component>> components;
    std::vector<boost::any> hints;

    for (auto const &component_datum : component_data)
    {
        auto const &preferred_size     = std::get<0>(component_datum);
        auto const &hint               = std::get<1>(component_datum);
        auto const &expected_placement = std::get<2>(component_datum);

        auto component = std::make_shared<mock_component>();
        EXPECT_CALL(*component, do_get_preferred_size())
            .WillRepeatedly(Return(preferred_size));
        EXPECT_CALL(*component, do_set_position(expected_placement.origin));
        EXPECT_CALL(*component, do_set_size(expected_placement.size));

        components.push_back(component);
        hints.push_back(hint);
    }

    auto lyt = munin::make_compass_layout();

    (*lyt)(components, hints, container_size);
}

INSTANTIATE_TEST_CASE_P(
    single_components_are_laid_out_on_their_edges,
    compass_layouts,
    ValuesIn(
    {
        compass_layout_test_data {{
            compass_layout_component_data {
                { 5, 5 },
                boost::any(munin::compass_layout::heading::centre),
                { { 0, 0 }, { 10, 10 } }
            }},
            { 10, 10 }
        },

        compass_layout_test_data {{
            compass_layout_component_data {
                { 5, 5 },
                boost::any(munin::compass_layout::heading::north),
                { { 0, 0 }, { 10, 5 } }
            }},
            { 10, 10 }
        },

        compass_layout_test_data {{
            compass_layout_component_data {
                { 5, 5 },
                boost::any(munin::compass_layout::heading::south),
                { { 0, 5 }, { 10, 5 } }
            }},
            { 10, 10 }
        },

        compass_layout_test_data {{
            compass_layout_component_data {
                { 5, 5 },
                boost::any(munin::compass_layout::heading::west),
                { { 0, 0 }, { 5, 10 } }
            }},
            { 10, 10 }
        },

        compass_layout_test_data {{
            compass_layout_component_data {
                { 5, 5 },
                boost::any(munin::compass_layout::heading::east),
                { { 5, 0 }, { 5, 10 } }
            }},
            { 10, 10 }
        }
    })
);
