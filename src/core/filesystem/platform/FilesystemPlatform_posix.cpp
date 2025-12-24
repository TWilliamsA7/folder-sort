// src/core/filesystem/platform/FilesystemPlatform_posix.cpp

#include "core/filesystem/platform/FilesystemPlatform.hpp"

bool fs_platform::is_hidden(const std::filesystem::path& p) {
    auto name = p.filename().string();
    return !name.empty() && name[0] == '.';
}
