// src/core/actions/MoveAction.cpp

#include "core/actions/MoveAction.hpp"

MoveAction::MoveAction(FileInfo file, std::filesystem::path destination) : Action(file), destination_(std::move(destination)) {}

ActionType MoveAction::type() const { return ActionType::MOVE; }

std::filesystem::path MoveAction::destination() const { return destination_; }

std::string MoveAction::describe() const {
    return "Moving " + file_.filename() + " from " + file_.path.string() + " to " + destination_.string();
}
