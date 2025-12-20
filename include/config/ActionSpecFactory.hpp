// include/config/ActionSpecFactory.hpp

#pragma once

#include <memory>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include "core/actions/ActionSpec.hpp"

class ActionSpecFactory {
    public:
        static ActionSpec build(const YAML::Node& node);
};