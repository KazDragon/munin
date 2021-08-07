#include "mock/component.hpp"

std::shared_ptr<mock_component> make_mock_component()
{
    return std::make_shared<testing::NiceMock<mock_component>>();
}
