// tests/core/conditions/NameConditionTests.cpp

#include <gtest/gtest.h>
#include "core/conditions/NameCondition.hpp"

TEST(NameConditionTests, MatchesBasicName) {
    const std::regex pattern("apple");
    NameCondition cond(pattern);

    FileInfo file1, file2;
    file1.path = "apple.txt";
    file2.path = "banana.txt";

    EXPECT_EQ(cond.check(file1), true);
    EXPECT_EQ(cond.check(file2), false);
}

TEST(NameConditionTests, MatchesComplexPattern) {
    const std::regex pattern(R"(^apple...+o)");
    NameCondition cond(pattern);

    FileInfo file1, file2, file3;
    file1.path = "apple32dooo.txt";
    file2.path = "apple23dd.txt";
    file3.path = "banana.txt";

    EXPECT_EQ(cond.check(file1), true);
    EXPECT_EQ(cond.check(file2), false);
    EXPECT_EQ(cond.check(file3), false);
}