// include/core/actions/ActionFactory.hpp

#pragma once

#include <memory>
#include <stdexcept>
#include "core/actions/Action.hpp"
#include "core/actions/ActionSpec.hpp"

// Action Headers
#include "core/actions/MoveAction.hpp"
#include "core/actions/RenameAction.hpp"
#include "core/actions/DeleteAction.hpp"


class ActionFactory {
    public:
        static std::unique_ptr<Action> create(const ActionSpec& spec);
};