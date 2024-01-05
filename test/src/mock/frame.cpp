#include "mock/frame.hpp"

std::shared_ptr<mock_frame> make_mock_frame()
{
  return std::make_shared<testing::NiceMock<mock_frame>>();
}
