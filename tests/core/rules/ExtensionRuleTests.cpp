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

TEST(ExtensionRuleTests, MatchesCaseInsensitiveExtension) {
    ExtensionRule rule(".TXT", "out/");

    FileInfo file;
    file.path = "report.txt";

    auto actions = rule.apply(file);
    EXPECT_EQ(actions.size(), 1);
}

TEST(ExtensionRuleTests, DoesNothingForNonMatchingExtension) {
    ExtensionRule rule(".pdf", "out/");

    FileInfo file;
    file.path = "image.png";

    auto actions = rule.apply(file);
    EXPECT_TRUE(actions.empty());
}

TEST(ExtensionRuleTests, NormalizesExtensionWithoutDot) {
    ExtensionRule rule("log", "logs/");

    FileInfo file;
    file.path = "system.log";

    auto actions = rule.apply(file);
    EXPECT_EQ(actions.size(), 1);
}


