// src/core/rules/Rule.cpp

#include "core/rules/Rule.hpp"

Rule::Rule(std::string name, std::vector<std::unique_ptr<Condition>> conditions, std::vector<ActionSpec> actions
    ) : name_(std::move(name)), conditions_(std::move(conditions)), actions_(std::move(actions)) {}

bool Rule::matches(const FileInfo& file) const {
    for (const auto& condition : conditions_) {
        if (!condition->check(file)) {
            return false;
        }
    }
    return true;
}


const std::string Rule::name() const { return name_; }
const std::vector<std::unique_ptr<Condition>> Rule::conditions() const { return conditions_; }
const std::vector<ActionSpec>& Rule::actions() const { return actions_; }
