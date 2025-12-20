// src/core/actions/RenameAction.cpp

#include "core/actions/RenameAction.hpp"

RenameAction::RenameAction(std::string new_name) : Action(), new_name_(new_name) {}
RenameAction::RenameAction(FileInfo* file, std::string new_name) : Action(file), new_name_(new_name) {}

ActionType RenameAction::type() const { return ActionType::RENAME; }

std::string RenameAction::describe() const {
    const FileInfo* file = this->getFile();

    if (file == nullptr) {
        return "[RENAME] file not specified";
    }

    return "Renaming" + file->filename() + " -> " + new_name_;
}

std::string RenameAction::new_name() const { return new_name_; }
