// tests/core/rules/ExtensionRuleTests.cpp

#include <gtest/gtest.h>
#include "core/rules/ExtensionRule.hpp"

TEST(ExtensionRuleTests, MatchesCorrectExtension) {
    ExtensionRule rule(".txt", "out/");

    FileInfo file;
    file.path = "example.txt";

    auto actions = rule.apply(file);

    ASSERT_EQ(actions.size(), 1);
    EXPECT_EQ(actions[0]->type(), ActionType::MOVE);
}