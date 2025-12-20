// tests/core/rules/RuleEngineTests.cpp

#include <gtest/gtest.h>
#include "core/rules/RuleEngine.hpp"
#include "core/rules/ExtensionRule.hpp"

TEST(RuleEngineTests, AggregatesActionsFromMultipleRules) {
    RuleEngine engine;

    engine.addRule(std::make_unique<ExtensionRule>(".txt", "text/"));
    engine.addRule(std::make_unique<ExtensionRule>(".txt", "backup/"));

    FileInfo file;
    file.path = "notes.txt";

    auto actions = engine.evaluate(file);

    ASSERT_EQ(actions.size(), 2);
}