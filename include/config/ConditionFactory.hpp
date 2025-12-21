// include/config/ConditionFactory.hpp

#pragma once

#include <memory>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include "core/conditions/Condition.hpp"

// Condition Headers
#include "core/conditions/ExtensionCondition.hpp"


class ConditionFactory {
    public:
        static std::vector<std::unique_ptr<Condition>> build(const YAML::Node& node);
};
