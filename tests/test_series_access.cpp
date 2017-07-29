#include "gtest/gtest.h"

#include "../include/paisano/paisano.hpp"

TEST(SeriesAccess, test_integer_access)
{
	std::vector<int> v{1, 2, 3};

    paisano::Series<int> s(v);

    EXPECT_EQ(s[1], 2);

    try {
        std::cout << s[3];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_custom_range_index_access)
{
	std::vector<int> v{1, 2, 3};
    paisano::RangeIndex i(-100, 200, 100);

    paisano::Series<int> s(v, i);

    EXPECT_EQ(s[-100], 1);
    EXPECT_EQ(s[0], 2);
    EXPECT_EQ(s[100], 3);

    try {
        std::cout << s[200];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}


TEST(SeriesAccess, test_string_access)
{
	std::vector<int> v{1, 2, 3};
	std::vector<std::string> i{"a", "b", "c"};

    paisano::Series<int, std::string> s(v, i);

    EXPECT_EQ(s["b"], 2);

    try {
        std::cout << s["d"];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
