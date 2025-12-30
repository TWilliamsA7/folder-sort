// include/core/actions/ActionExecutor.hpp

#pragma once

#include <vector>
#include "logging/Logger.hpp"
#include "Action.hpp"

class ActionExecutor {
    public:

        // * Processor for Actions
        explicit ActionExecutor(bool dry_run = false, bool verbose = false);

        // * Given an action, execute it
        void execute(const Action&, FileInfo&) const;

        // * Given a vector of actions, execute each action
        void execute(std::vector<std::unique_ptr<Action>>, FileInfo&) const;

    private:
        // * If True, no actions will actually take place
        bool dry_run_;
        bool verbose_;
};