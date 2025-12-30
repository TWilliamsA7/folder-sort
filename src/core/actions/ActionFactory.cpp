// src/core/actions/ActionFactory.cpp

#include "core/actions/ActionFactory.hpp"

std::unique_ptr<Action> ActionFactory::create(const ActionSpec& spec) {
    switch (spec.type)
    {
        case ActionType::MOVE: {
            auto it = spec.params.find("to");
            if (it == spec.params.end()) {
                throw std::runtime_error("MOVE action requires 'to' parameter");
            }
            return std::make_unique<MoveAction>(it->second);
        }

        case ActionType::RENAME: {
            auto it = spec.params.find("pattern");
            if (it == spec.params.end()) {
                throw std::runtime_error("RENAME action requires 'pattern' parameter");
            }
            return std::make_unique<RenameAction>(it->second);
        }

        case ActionType::REMOVE: {
            return std::make_unique<DeleteAction>();
        }

        default:
            throw std::runtime_error("Unknown ActionType");
    }
}