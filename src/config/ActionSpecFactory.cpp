// src/config/ActionSpecFactory.cpp

#include "config/ActionSpecFactory.hpp"

ActionSpec ActionSpecFactory::build(const YAML::Node& node) {
    if (!node["action"]) {
        throw std::runtime_error("'then' block requires 'action'");
    }

    const std::string action = node["action"].as<std::string>();

    if (action == "move") {
        if (!node["to"]) {
            throw std::runtime_error("'move' action requires 'to'");
        }

        const std::string target = node["to"].as<std::string>();

        const std::unordered_map<std::string, std::string> params = {{ "to", target }};

        ActionSpec spec(ActionType::MOVE, params);

        return spec;
    }

    if (action == "delete") {
        ActionSpec spec(ActionType::REMOVE, {});
        return spec;
    }

    if (action == "rename") {
        if (!node["pattern"]) {
            throw std::runtime_error("'rename' action requires 'pattern'");
        }

        const std::string pattern = node["pattern"].as<std::string>();

        const std::unordered_map<std::string, std::string> params = {{ "pattern", pattern }};

        ActionSpec spec(ActionType::RENAME, params);

        return spec;
    }

    if (action == "copy") {
        if (!node["to"]) {
            throw std::runtime_error("'copy' action requires 'to'");
        }

        const std::string target = node["to"].as<std::string>();

        const std::unordered_map<std::string, std::string> params = {{ "to", target }};

        ActionSpec spec(ActionType::COPY, params);

        return spec;
    }

    throw std::runtime_error("Unknown action type: " + action);
}