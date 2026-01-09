// src/core/actions/CopyAction.cpp

#include "core/actions/CopyAction.hpp"

CopyAction::CopyAction(std::filesystem::path destination) : Action(), destination_(destination) {};

ActionType CopyAction::type() const { return ActionType::MOVE; }

std::filesystem::path CopyAction::destination() const { return destination_; }

const std::string CopyAction::describe() const {
    const auto utf8 = destination_.u8string();
    return "Copy to " + std::string(utf8.begin(), utf8.end());
}

const std::string CopyAction::describe(const FileInfo& file) const {
    const auto utf8 = destination_.u8string();
    return "Copy " + file.filename() + " to " + std::string(utf8.begin(), utf8.end());
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
