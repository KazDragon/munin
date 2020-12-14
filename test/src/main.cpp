#include <gtest/gtest.h>

// This is a (hopefully) temporary hack for MSVC, which for some reason does not automatically
// detect or run tests if GoogleMock is being used.
int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
