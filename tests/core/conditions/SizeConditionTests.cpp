// tests/core/conditions/SizeConditionTests.cpp

#include <gtest/gtest.h>
#include "core/conditions/SizeCondition.hpp"

TEST(SizeConditionTests, MatchesSmallerSizes) {
    SizeCondition cond(5012, Comparison::LESS);

    FileInfo file1, file2, file3;
    file1.size = 2124;
    file2.size = 1e5;
    file3.size = 4587;

    EXPECT_EQ(cond.check(file1), true);
    EXPECT_EQ(cond.check(file2), false);
    EXPECT_EQ(cond.check(file3), true);
}

TEST(SizeConditionTests, MatchesLargerSizes) {
    SizeCondition cond(5012, Comparison::GREATER);

    FileInfo file1, file2, file3;
    file1.size = 2124;
    file2.size = 1e5;
    file3.size = 4587;

    EXPECT_EQ(cond.check(file1), false);
    EXPECT_EQ(cond.check(file2), true);
    EXPECT_EQ(cond.check(file3), false);
}