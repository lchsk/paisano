#include "gtest/gtest.h"

#include "../include/paisano/paisano.hpp"

TEST(Basic, example1)
{
    paisano::Series<int> s(std::vector<int>{1, 2, 3});

    EXPECT_EQ(1, 1);
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
