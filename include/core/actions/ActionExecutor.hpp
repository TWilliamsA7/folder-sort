// include/core/actions/ActionExecutor.hpp

#pragma once

#include <vector>
#include <spdlog/spdlog.h>
#include "Action.hpp"

class ActionExecutor {
    public:

        // * Processor for Actions
        explicit ActionExecutor(bool dry_run = false);

        // * Given an action, execute it
        void execute(const Action&, const FileInfo&) const;

        // * Given a vector of actions, execute each action
        void execute(std::vector<std::unique_ptr<Action>>, const FileInfo&) const;

    private:
        // * If True, no actions will actually take place
        bool dry_run_;
};