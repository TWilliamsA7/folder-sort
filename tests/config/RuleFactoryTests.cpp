// tests/config/RuleFactoryTests.cpp

#include <gtest/gtest.h>

#include "config/RuleFactory.hpp"

TEST(RuleFactoryTests, BuildsValidRule) {
    const char* yaml = R"(
        rules:
            - name: "Archive Text Documents"
              when:
                extension: ".txt"
              then:
                - action: move
                  to: "Text/"
    )";

    YAML::Node root = YAML::Load(yaml);
    auto rules = RuleFactory::buildRules(root);

    EXPECT_EQ(rules.size(), 1);
}

TEST(RuleFactoryTests, ThrowsForMalformedRule) {
    const char* yaml = R"(
        rules:
            - name: "Archive Text Documents"
              then:
                - action: move
                  to: "Text/"
    )";

    YAML::Node root = YAML::Load(yaml);
    EXPECT_THROW(RuleFactory::buildRules(root), std::exception);
}

TEST(RuleFactoryTests, BuildsUnnamedRule) {
    const char* yaml = R"(
        rules:
              - when:
                 extension: ".txt"
                then:
                 - action: move
                   to: "Text/"
    )";

    YAML::Node root = YAML::Load(yaml);
    auto rules = RuleFactory::buildRules(root);

    EXPECT_EQ(rules.size(), 1);
}

TEST(RuleFactoryTests, BuildsMultiActionRules) {
    const char* yaml = R"(
        rules:
            - name: "Name and move images"
              when:
                extension: [".png", ".jpg"]
              then:
                - action: rename
                  pattern: "{name}_{date}"
                - action: move
                  to: "Text/"
    )";

    YAML::Node root = YAML::Load(yaml);
    auto rules = RuleFactory::buildRules(root);

    ASSERT_EQ(rules.size(), 1);
    EXPECT_EQ(rules.at(0)->actions().size(), 2);
}