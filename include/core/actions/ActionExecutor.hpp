// include/core/actions/ActionExecutor.hpp

#pragma once

#include "Action.hpp"
#include <vector>

class ActionExecutor {
    public:

        // * Processor for Actions
        explicit ActionExecutor(bool dry_run = false);

        // * Given an action, execute it
        void execute(const Action&) const;

        // * Given a vector of actions, execute each action
        void execute(std::vector<std::unique_ptr<Action>>) const;

    private:
        // * If True, no actions will actually take place
        bool dry_run_;
};