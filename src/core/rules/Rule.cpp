// src/core/rules/Rule.cpp

#include "core/rules/Rule.hpp"

Rule::Rule(std::string name, std::vector<std::unique_ptr<Condition>> conditions, std::vector<std::unique_ptr<Action>> actions
    ) : name_(std::move(name)), conditions_(std::move(conditions)), actions_(std::move(actions)) {}

const std::string Rule::name() const { return name_; }
const std::vector<std::unique_ptr<Condition>> Rule::conditions() const { return conditions_; }
const std::vector<std::unique_ptr<Action>> Rule::actions() const { return actions_; }
