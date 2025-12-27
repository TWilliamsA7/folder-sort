// tests/flows/FlowTests.cpp

#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

#include "config/RuleFactory.hpp"
#include "core/rules/RuleEngine.hpp"
#include "core/actions/ActionExecutor.hpp"
#include "../tests/flows/FlowTest.hpp"

TEST_F(FlowTest, RuleTriggersActions) {  
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

    ASSERT_EQ(rules.size(), 1);

    RuleEngine engine;
    for (auto& rule : rules) {
        engine.addRule(std::move(rule));
    }

    FileInfo file;
    file.path = "dog.txt";

    auto actions = engine.evaluate(file);

    ASSERT_EQ(actions.size(), 1);
    ASSERT_EQ(actions.at(0)->type(), ActionType::MOVE);

    const std::string descr = actions.at(0)->describe(file);

    ActionExecutor exec(/* dry_run=*/true, true);

    EXPECT_NO_THROW(exec.execute(std::move(actions), file));

    EXPECT_TRUE(std::filesystem::exists(logFile));

    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string contents(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    EXPECT_NE(contents.find(descr), std::string::npos)
        << "Actual contents: " << contents;

    
}