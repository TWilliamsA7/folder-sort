// src/core/actions/ActionExecutor.cpp

#include "core/actions/ActionExecutor.hpp"

namespace {
    const char *kLoggerName = "app.actions.executor";
}

ActionExecutor::ActionExecutor(bool dry_run) : dry_run_(dry_run) {
    auto log = logging::Logger::Get(kLoggerName);
}

void ActionExecutor::execute(const Action& action, const FileInfo& file) const {
    auto log = logging::Logger::Get(kLoggerName);

    if (dry_run_) {
        log->info("[DRY RUN] {}", action.describe(file));
        return;
    }

    action.execute(file);
}

void ActionExecutor::execute(std::vector<std::unique_ptr<Action>> actions, const FileInfo& file) const {
    for (const auto& action : actions) {
        execute(*action, file);
    }
}
