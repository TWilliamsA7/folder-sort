// src/core/actions/DeleteAction.cpp

#include "core/actions/DeleteAction.hpp"

DeleteAction::DeleteAction() : Action() {}

ActionType DeleteAction::type() const { return ActionType::DELETE; }

std::string DeleteAction::describe() const {
    return "[DELETE]";
};

std::string DeleteAction::describe(FileInfo& file) const {
    return "Deleting " + file.path.string();
};

void DeleteAction::execute(FileInfo& file) const {
    std::filesystem::remove(file.path);
}
