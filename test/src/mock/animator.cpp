#include "mock/animator.hpp"

std::shared_ptr<mock_animator> make_mock_animator()
{
    return std::make_shared<testing::NiceMock<mock_animator>>();
}
