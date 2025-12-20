// src/config/RuleFactory.cpp

#include "config/RuleFactory.hpp"
#include "config/ConditionFactory.hpp"
#include "config/ActionSpecFactory.hpp"

std::vector<std::unique_ptr<Rule>> RuleFactory::buildRules(const YAML::Node& root) {
    std::vector<std::unique_ptr<Rule>> rules;

    for (const auto& ruleNode : root["rules"]) {
        if (!ruleNode["when"] || !ruleNode["then"]) {
            throw std::runtime_error("Each rule must have 'when' and 'then' properties");
        }

        auto condition = ConditionFactory::build(ruleNode["when"]);
        auto action = ActionSpecFactory::build(ruleNode["then"]);

        rules.push_back(
            std::make_unique<Rule>(
                ruleNode["name"] ? ruleNode["name"].as<std::string>() : "",
                std::move(condition),
                std::move(action)
            )
        );
    }

    return rules;
}