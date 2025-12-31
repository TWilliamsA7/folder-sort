// include/core/actions/ActionSpec.hpp

#pragma once

#include <string>
#include <unordered_map>


enum class ActionType {
    MOVE,
    RENAME,
    REMOVE,
    COPY
};

struct ActionSpec {
    ActionType type;
    std::unordered_map<std::string, std::string> params;

    ActionSpec(ActionType type_, std::unordered_map<std::string, std::string> params_) 
        : type(type_), params(params_) {}; 
};