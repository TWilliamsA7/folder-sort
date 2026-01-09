// src/core/actions/DeleteAction.cpp

#include "core/actions/DeleteAction.hpp"

DeleteAction::DeleteAction() : Action() {}

ActionType DeleteAction::type() const { return ActionType::REMOVE; }

const std::string DeleteAction::describe() const {
    return "[DELETE]";
};

const std::string DeleteAction::describe(const FileInfo& file) const {
    const auto utf8 = file.path.u8string();
    return "Deleting " + std::string(utf8.begin(), utf8.end());
};

void DeleteAction::execute(FileInfo& file) const {
    std::filesystem::remove(file.path);
}
