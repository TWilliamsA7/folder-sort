// include/core/actions/ActionExecutor.hpp

#pragma once

#include "Action.hpp"
#include <vector>

class ActionExecutor {
    public:
        // * Given a vector of actions, execute each action
        void execute(std::vector<std::unique_ptr<Action>>) const;
};