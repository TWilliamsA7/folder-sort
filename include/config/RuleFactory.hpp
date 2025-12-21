// include/config/RuleFactory.hpp

#pragma once

#include <memory>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "core/rules/Rule.hpp"

// * Static Class used to generate a set of rules from a YAML config
class RuleFactory {
    public:
        // * Generate a set of rules from YAML
        static std::vector<std::unique_ptr<Rule>> buildRules(const YAML::Node& root);
};

