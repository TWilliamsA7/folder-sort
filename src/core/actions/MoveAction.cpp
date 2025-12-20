// src/core/actions/MoveAction.cpp

#include "core/actions/MoveAction.hpp"

MoveAction::MoveAction(std::filesystem::path destination) : Action(), destination_(destination) {};

ActionType MoveAction::type() const { return ActionType::MOVE; }

std::filesystem::path MoveAction::destination() const { return destination_; }

std::string MoveAction::describe() const {
    return "Move to " + destination_.string();
}

std::string MoveAction::describe(FileInfo& file) const {
    return "Move " + file.filename() + " to " + destination_.string();
}

void MoveAction::execute(FileInfo& file) const {
    std::filesystem::path dest = destination_ / file.filename();
    std::filesystem::create_directories(dest.parent_path());
    std::filesystem::rename(file.path, dest);
}
