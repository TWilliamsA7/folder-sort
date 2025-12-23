// include/core/filesystem/platform/FilesystemPlatform.hpp

#pragma once
#include <filesystem>

namespace fs_platform {
    bool is_hidden(const std::filesystem::path& p);
}