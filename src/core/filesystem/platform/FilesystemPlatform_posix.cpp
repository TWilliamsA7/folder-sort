// src/core/filesystem/platform/FilesystemPlatform_posix.cpp

#include "core/filesystem/platform/FilesystemPlatform.hpp"

bool fs_platform::is_hidden(const std::filesystem::path& p) {
    const auto utf8 = p.filename().u8string();
    auto name = std::string(utf8.begin(), utf8.end());
    return !name.empty() && name[0] == '.';
}
