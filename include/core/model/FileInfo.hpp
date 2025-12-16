// include/core/model/FileInfo.hpp

#pragma once

#include <filesystem>
#include <optional>
#include <string>

struct FileInfo {
    std::filesystem::path path;
    std::optional<std::uintmax_t> size;
    std::optional<std::filesystem::file_time_type> last_modified;

    std::string filename() const { return path.filename().string(); }
    std::string extension() const { return path.extension().string(); }
};