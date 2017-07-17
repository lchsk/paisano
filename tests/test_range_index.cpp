#include "gtest/gtest.h"

#include "../include/paisano/paisano.hpp"

TEST(RangeIndex, test_create_range_index)
{
    paisano::RangeIndex ri(0, 100, 1);
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
