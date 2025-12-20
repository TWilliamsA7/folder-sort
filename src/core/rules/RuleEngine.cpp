// src/core/rules/RuleEngine.cpp

#include "core/rules/RuleEngine.hpp"

void RuleEngine::addRule(std::unique_ptr<Rule> rule) {
    rules_.push_back(std::move(rule));
}

std::vector<std::unique_ptr<Action>> RuleEngine::evaluate(const FileInfo& file) const {

    std::vector<std::unique_ptr<Action>> actions;

    for (const auto& rule : rules_) {
        std::vector<std::unique_ptr<Action>> rule_actions = rule->apply(file);
        actions.insert(
            actions.end(), 
            std::make_move_iterator(rule_actions.begin()), 
            std::make_move_iterator(rule_actions.end())
        );
    }

    return actions;
}