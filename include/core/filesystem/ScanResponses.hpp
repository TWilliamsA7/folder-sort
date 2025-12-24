// include/core/filesystem/ScanResponses.hpp

#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <string>

enum class ScanErrorType {
    PermissionDenied,
    NotFound,
    PathTooLong,
    ReparsePoint,     
    IoError,
    Unknown
};

struct ScanError {
    std::filesystem::path path;
    ScanErrorType type;
    std::error_code ec;

    std::string message() const { return ec.message(); }
};

enum class FileType {
    RegularFile,
    Directory,
    Symlink,
    Other
};

struct FileInfo {
    std::filesystem::path path;
    FileType type;
    std::optional<std::uintmax_t> size;
    std::optional<std::filesystem::file_time_type> last_modified;

    std::string filename() const { return path.filename().string(); }
    std::string extension() const { return path.extension().string(); }
};

struct ScanResult {
    std::vector<FileInfo> files;
    std::vector<ScanError> errors;

    bool has_errors() const { return !errors.empty(); }
};
