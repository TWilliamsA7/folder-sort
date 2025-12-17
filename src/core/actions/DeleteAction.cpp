// src/core/actions/DeleteAction.cpp

#include "core/actions/DeleteAction.hpp"

DeleteAction::DeleteAction(FileInfo file) : Action(file) {}

ActionType DeleteAction::type() const { return ActionType::DELETE; }

std::string DeleteAction::describe() const {
    return "Deleting" + file_.filename() + " : " + file_.path.string();
}
