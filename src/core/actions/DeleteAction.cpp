// src/core/actions/DeleteAction.cpp

#include "core/actions/DeleteAction.hpp"

DeleteAction::DeleteAction() : Action() {}

DeleteAction::DeleteAction(FileInfo* file) : Action(file) {}

ActionType DeleteAction::type() const { return ActionType::DELETE; }

std::string DeleteAction::describe() const {
    const FileInfo* file = this->getFile();

    if (file == nullptr) {
        return "[DELETE] File not specified";
    }

    return "Deleting" + file->filename() + " : " + file->path.string();
}
