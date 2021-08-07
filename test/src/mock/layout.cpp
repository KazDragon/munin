#include "mock/layout.hpp"
#include <boost/make_unique.hpp>

std::unique_ptr<mock_layout> make_mock_layout()
{
    return boost::make_unique<testing::NiceMock<mock_layout>>();
}
