#include "mock/layout.hpp"

#include <memory>

std::unique_ptr<mock_layout> make_mock_layout()
{
    return std::make_unique<testing::NiceMock<mock_layout>>();
}
