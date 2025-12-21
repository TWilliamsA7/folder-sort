// src/core/actions/ActionExecutor.cpp

#include "core/actions/ActionExecutor.hpp"

ActionExecutor::ActionExecutor(bool dry_run) : dry_run_(dry_run) {}

void ActionExecutor::execute(const Action& action, const FileInfo& file) const {
    if (dry_run_) {
        spdlog::info("[DRY RUN] {}", action.describe(file));
        return;
    }

    action.execute(file);
}

void ActionExecutor::execute(std::vector<std::unique_ptr<Action>> actions, const FileInfo& file) const {
    for (const auto& action : actions) {
        execute(*action, file);
    }
}
