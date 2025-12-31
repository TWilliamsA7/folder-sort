// src/core/actions/CopyAction.cpp

#include "core/actions/CopyAction.hpp"

CopyAction::CopyAction(std::filesystem::path destination) : Action(), destination_(destination) {};

ActionType CopyAction::type() const { return ActionType::MOVE; }

std::filesystem::path CopyAction::destination() const { return destination_; }

const std::string CopyAction::describe() const {
    return "Copy to " + destination_.string();
}

const std::string CopyAction::describe(const FileInfo& file) const {
    return "Copy " + file.filename() + " to " + destination_.string();
}

void CopyAction::execute(FileInfo& file) const {
    std::filesystem::path dest = destination_ / file.filename();
    std::filesystem::create_directories(dest.parent_path());
    std::filesystem::copy(
        file.path,
        dest,
        std::filesystem::copy_options::recursive |
        std::filesystem::copy_options::update_existing
    );

    file.path = dest;
}
