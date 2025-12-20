// src/core/actions/ActionExecutor.cpp

#include "core/actions/ActionExecutor.hpp"

#include "core/actions/MoveAction.hpp"
#include "core/actions/DeleteAction.hpp"
#include "core/actions/RenameAction.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>


ActionExecutor::ActionExecutor(bool dry_run) : dry_run_(dry_run) {}

void ActionExecutor::execute(const Action& action) const {
    switch (action.type()) {

        // Define source and destination -> create/ensure directory -> move
        case ActionType::MOVE: {
            const auto* move = static_cast<const MoveAction*>(&action);

            if (dry_run_) {
                spdlog::info("[DRY RUN] {}", move->describe());
                return;
            } else {
                spdlog::info(move->describe());
            }

            std::filesystem::path source = move->file().path;
            std::filesystem::path destination = move->destination() / move->file().filename();
            
            
            std::filesystem::create_directories(destination.parent_path());
            std::filesystem::rename(source, destination);
            break;
        }

        // Log deletion -> perform deletion
        case ActionType::DELETE: {
            const auto* del = static_cast<const DeleteAction*>(&action);
            
            if (dry_run_) {
                spdlog::info("[DRY RUN] {}", del->describe());
                return;
            } else {
                spdlog::info(del->describe());
            }

            std::filesystem::remove(del->file().path);
            break;
        }

        // Log rename -> rebuild new path -> perform rename
        case ActionType::RENAME: {
            const auto* ren = static_cast<const RenameAction*>(&action);
            
            if (dry_run_) {
                spdlog::info("[DRY RUN] {}", ren->describe());
                return;
            } else {
                spdlog::info(ren->describe());
            }

            std::filesystem::rename(
                ren->file().path,
                ren->file().path.parent_path() / ren->new_name()
            );
            break;
        }
        
        default:
            spdlog::error("Unknown Action!");
    }
}

void ActionExecutor::execute(std::vector<std::unique_ptr<Action>> actions) const {
    for (const auto& action : actions) {
        execute(*action);
    }
}
