// src/core/actions/ActionExecutor.cpp

#include "core/actions/ActionExecutor.hpp"

#include "core/actions/MoveAction.hpp"
#include "core/actions/DeleteAction.hpp"
#include "core/actions/RenameAction.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

void ActionExecutor::execute(std::vector<std::unique_ptr<Action>> actions) const {
    for (const auto& action : actions) {
        switch (action->type()) {

            // Define source and destination -> create/ensure directory -> move
            case ActionType::MOVE: {
                const auto* move = static_cast<const MoveAction*>(action.get());

                std::filesystem::path source = move->file().path;
                std::filesystem::path destination = move->destination() / move->file().filename();
                
                spdlog::info("Moving {} -> {}", 
                            source.string(),
                            destination.string());
                
                std::filesystem::create_directories(destination.parent_path());
                std::filesystem::rename(source, destination);
                break;
            }

            // Log deletion -> perform deletion
            case ActionType::DELETE: {
                const auto* del = static_cast<const DeleteAction*>(action.get());
                spdlog::info("Deleting {}", del->file().path.string());
                std::filesystem::remove(del->file().path);
                break;
            }

            // Log rename -> rebuild new path -> perform rename
            case ActionType::RENAME: {
                const auto* ren = static_cast<const RenameAction*>(action.get());
                spdlog::info("Renaming {} -> {}", ren->file().filename(), ren->new_name());
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
}

