// include/config/ConditionFactory.hpp

#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>
#include <charconv>
#include <yaml-cpp/yaml.h>
#include <spdlog/fmt/fmt.h>
#include "core/conditions/Condition.hpp"


// Condition Headers
#include "core/conditions/ExtensionCondition.hpp"
#include "core/conditions/SizeCondition.hpp"


class ConditionFactory {
    public:
        static std::vector<std::unique_ptr<Condition>> build(const YAML::Node& node);
    private:
        static std::unique_ptr<ExtensionCondition> buildExtensionCondition(const YAML::Node& node);
        static std::unique_ptr<SizeCondition> buildSizeCondition(const YAML::Node& node);

        static std::uintmax_t getSize(std::string_view inp);
};
