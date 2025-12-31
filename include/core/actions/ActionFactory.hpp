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
#include "core/actions/CopyAction.hpp"

// Logging Header
#include "logging/Logger.hpp"


class ActionFactory {
    public:
        static std::unique_ptr<Action> create(const ActionSpec& spec, const std::filesystem::path& root_dir, const int rule_file_count);
    private:
        static std::unique_ptr<MoveAction> createMoveAction(const ActionSpec& spec, const std::filesystem::path& root_dir);
        static std::unique_ptr<RenameAction> createRenameAction(const ActionSpec& spec, const int rule_file_count);
        static std::unique_ptr<CopyAction> createCopyAction(const ActionSpec& spec, const std::filesystem::path& root_dir);
};