// src/core/actions/ActionFactory.cpp

#include "core/actions/ActionFactory.hpp"

namespace {
    const char* kLoggerName = "app.action.factory";
};

std::unique_ptr<Action> ActionFactory::create(const ActionSpec& spec, const std::filesystem::path& root_dir) {
    auto log = logging::Logger::Get(kLoggerName);
    
    switch (spec.type) {
        case ActionType::MOVE: {
            return createMoveAction(spec, root_dir);
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

std::unique_ptr<Action> ActionFactory::createMoveAction(const ActionSpec& spec, const std::filesystem::path& root_dir) {
    auto log = logging::Logger::Get(kLoggerName);
    
    auto it = spec.params.find("to");
    if (it == spec.params.end()) {
        log->error("MOVE action missing 'to' parameter");
        throw std::runtime_error("MOVE action requires 'to' parameter");
    }

    // Attempt to create a path using provided to parameter
    std::filesystem::path p(it->second);

    // If the path is absolute, use it as is
    if (!p.is_absolute()) {
        p = root_dir / it->second;
    }

    return std::make_unique<MoveAction>(p);
}