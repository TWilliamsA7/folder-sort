// src/config/ConditionFactory.cpp

#include "config/ConditionFactory.hpp"

std::unique_ptr<Condition> ConditionFactory::build(const YAML::Node& node) {
    if (node["extension"]) {
        if (node["extension"].IsSequence()) {
            return std::make_unique<ExtensionCondition>(
                node["extension"].as<std::vector<std::string>>()
            );
        }
        return std::make_unique<ExtensionCondition>(
            std::vector<std::string>{ node["extension"].as<std::string>() }
        );
    }

    throw std::runtime_error("Unsupported condition type");
}