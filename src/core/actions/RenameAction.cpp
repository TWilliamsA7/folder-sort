// src/core/actions/RenameAction.cpp

#include "core/actions/RenameAction.hpp"

RenameAction::RenameAction(std::string new_name) : Action(), new_name_(new_name) {}

ActionType RenameAction::type() const { return ActionType::RENAME; }

std::string RenameAction::describe() const {
    return "Renaming [FILE] to " + new_name_;
}

std::string RenameAction::describe(FileInfo& file) const {
    return "Renaming " + file.filename() + " to " + new_name_;
}

void RenameAction::execute(FileInfo& file) const {
    std::filesystem::rename(
        file.path,
        file.path.parent_path() / new_name_
    );
}

std::string RenameAction::new_name() const { return new_name_; }
