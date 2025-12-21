// tests/config/ActionSpecFactoryTests.cpp

#include <gtest/gtest.h>

#include "config/ActionSpecFactory.hpp"

TEST (ActionSpecFactoryTests, BuildsExpectedAction) {
    const char* yaml = R"(
        action: move
        to: "Images/"
    )";

    YAML::Node root = YAML::Load(yaml);
    ActionSpec res = ActionSpecFactory::build(root);
    
    EXPECT_EQ(res.type, ActionType::MOVE);
    EXPECT_EQ(res.params.at("to"), "Images/");
}

TEST (ActionSpecFactoryTests, MissingActionThrows) {
    const char* yaml = R"()";

    YAML::Node root = YAML::Load(yaml);
    EXPECT_THROW(ActionSpecFactory::build(root), std::exception);
}

TEST (ActionSpecFactoryTests, MalformedActionThrows) {
        const char* yaml = R"(
            - action: move
              target: "Somewhere"
        )";

    YAML::Node root = YAML::Load(yaml);
    EXPECT_THROW(ActionSpecFactory::build(root), std::exception);
}