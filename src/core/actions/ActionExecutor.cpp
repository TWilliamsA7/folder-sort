// src/core/actions/ActionExecutor.cpp

#include "core/actions/ActionExecutor.hpp"

namespace {
    const char *kLoggerName = "app.actions.executor";
}

ActionExecutor::ActionExecutor(bool dry_run, bool verbose) : dry_run_(dry_run), verbose_(verbose) {
    auto log = logging::Logger::Get(kLoggerName);
}

void ActionExecutor::execute(const Action& action, FileInfo& file) const {

    auto log = logging::Logger::Get(kLoggerName);
    
    if (dry_run_) {
        log->info("[DRY RUN] {}", action.describe(file));
        return;
    } else {
        log->info(action.describe(file));
    }

    action.execute(file);
}

void ActionExecutor::execute(std::vector<std::unique_ptr<Action>> actions, FileInfo& file) const {
    for (const auto& action : actions) {
        execute(*action, file);
    }
}
