// src/core/actions/MoveAction.cpp

#include "core/actions/MoveAction.hpp"

MoveAction::MoveAction(std::filesystem::path destination) : Action(), destination_(destination) {};

MoveAction::MoveAction(FileInfo* file, std::filesystem::path destination) : Action(file), destination_(std::move(destination)) {}

ActionType MoveAction::type() const { return ActionType::MOVE; }

std::filesystem::path MoveAction::destination() const { return destination_; }

std::string MoveAction::describe() const {
    const FileInfo* file = this->getFile();

    if (file == nullptr) {
        return "[MOVE] File not specified";
    }

    return "Moving " + file->filename() + " : " + file->path.string() + " -> " + destination_.string();
}
