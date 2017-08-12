#include "gtest/gtest.h"

#include "../include/paisano/paisano.hpp"

TEST(SeriesAccess, test_integer_access)
{
	std::vector<int> v{1, 2, 3};

    paisano::Series<int> s(v);

    int i = s[1];

    EXPECT_EQ(i, 2);
    EXPECT_EQ(s[1], 2);

    try {
        std::cout << s[3];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_explicit_integer_access)
{
	std::vector<int> v{1, 2, 3};
	std::vector<int> i{1, 2, 3};

    paisano::Series<int, int> s(v, i);

    EXPECT_EQ(s[1], 2);

    try {
        std::cout << s[3];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_explicit_index_object_access)
{
	std::vector<int> v{1, 2, 3};

    paisano::Index<int> i({1,2,3});
    paisano::Series<int, int> s(v, i);

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

    EXPECT_EQ(s["b"], 2); // Test that access works using the string key


    EXPECT_EQ(s[0], 1); // Access using an integer index must still work

    try {
        std::cout << s["d"];

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_update_by_integer)
{
	std::vector<int> v{1, 2, 3};

    paisano::Series<int> s(v);

    EXPECT_EQ(s[1], 2);

    s[1] = 5;

    EXPECT_EQ(s[1], 5);

    try {
        s[10] = 10;

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_update_by_range_index_integer)
{
	std::vector<int> v{1, 2, 3};
    paisano::RangeIndex i(-100, 200, 100);

    paisano::Series<int> s(v, i);

    EXPECT_EQ(s[-100], 1);
    EXPECT_EQ(s[0], 2);
    EXPECT_EQ(s[100], 3);

    s[-100] = -100;
    s[0] = 0;
    s[100] = 100;

    EXPECT_EQ(s[-100], -100);
    EXPECT_EQ(s[0], 0);
    EXPECT_EQ(s[100], 100);

    try {
        s[1] = 1;

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

TEST(SeriesAccess, test_update_by_string)
{
	std::vector<int> v{1, 2, 3};
	std::vector<std::string> i{"a", "b", "c"};

    paisano::Series<int, std::string> s(v, i);

    EXPECT_EQ(s["b"], 2);

    s["b"] = 1;

    EXPECT_EQ(s["b"], 1);

    s[0] = -1; // Update by an integer key must still work

    EXPECT_EQ(s[0], -1);

    try {
        s["d"] = -1;

        FAIL();
    } catch (std::out_of_range) {
        SUCCEED();
    }
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
