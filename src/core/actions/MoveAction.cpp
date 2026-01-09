// src/core/actions/MoveAction.cpp

#include "core/actions/MoveAction.hpp"

MoveAction::MoveAction(std::filesystem::path destination) : Action(), destination_(destination) {};

ActionType MoveAction::type() const { return ActionType::MOVE; }

std::filesystem::path MoveAction::destination() const { return destination_; }

const std::string MoveAction::describe() const {
    const auto utf8 = destination_.u8string();
    return "Move to " + std::string(utf8.begin(), utf8.end());
}

const std::string MoveAction::describe(const FileInfo& file) const {
    const auto utf8 = destination_.u8string();
    return "Move " + file.filename() + " to " + std::string(utf8.begin(), utf8.end());
}

void MoveAction::execute(FileInfo& file) const {
    std::filesystem::path dest = destination_ / file.filename();
    std::filesystem::create_directories(dest.parent_path());
    std::filesystem::rename(file.path, dest);

    file.path = dest;
}
