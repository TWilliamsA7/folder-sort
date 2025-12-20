// src/config/ConditionFactory.cpp

#include "config/ConditionFactory.hpp"

std::vector<std::unique_ptr<Condition>> ConditionFactory::build(const YAML::Node& node) {

    std::vector<std::unique_ptr<Condition>> conditions;

    if (node["extension"]) {
        if (node["extension"].IsSequence()) {
            conditions.push_back(
                std::make_unique<ExtensionCondition>(
                    node["extension"].as<std::vector<std::string>>()
                )
            );
        } else {
            conditions.push_back(
                std::make_unique<ExtensionCondition>(
                    std::vector<std::string>{ node["extension"].as<std::string>() }
                )
            );
        }
        
    }

    return conditions;
}