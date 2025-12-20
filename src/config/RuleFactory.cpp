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

        std::vector<std::unique_ptr<Condition>> conditions = ConditionFactory::build(ruleNode["when"]);
        std::vector<ActionSpec> actionSpecs;
        for (const auto& actionNode : ruleNode["then"]) {
            actionSpecs.push_back(ActionSpecFactory::build(actionNode));
        }

        rules.push_back(
            std::make_unique<Rule>(
                ruleNode["name"] ? ruleNode["name"].as<std::string>() : "",
                std::move(conditions),
                std::move(actionSpecs)
            )
        );
    }

    return rules;
}