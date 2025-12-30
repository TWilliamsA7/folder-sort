// src/core/actions/DeleteAction.cpp

#include "core/actions/DeleteAction.hpp"

DeleteAction::DeleteAction() : Action() {}

ActionType DeleteAction::type() const { return ActionType::REMOVE; }

const std::string DeleteAction::describe() const {
    return "[DELETE]";
};

const std::string DeleteAction::describe(const FileInfo& file) const {
    return "Deleting " + file.path.string();
};

void DeleteAction::execute(const FileInfo& file) const {
    std::filesystem::remove(file.path);
}
