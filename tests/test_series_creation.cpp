#include "gtest/gtest.h"

#include "../include/paisano/paisano.hpp"

TEST(SeriesCreation, test_series_has_a_copy)
{
	std::vector<int> v{1, 2, 3};

    paisano::Series<int> s(v);

    EXPECT_EQ(s.data().size(), v.size());

    v.push_back(4);

    EXPECT_EQ(s.data().size(), 3);
    EXPECT_EQ(v.size(), 4);
}

TEST(SeriesCreation, test_series_created_with_explicit_index)
{
    std::vector<int> v{1, 2, 3};
    std::vector<std::string> i{"a", "b", "c"};

    paisano::Series<int, std::string> s(v, i);
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
