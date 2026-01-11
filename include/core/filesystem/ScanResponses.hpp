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

    std::string filename() const { 
        const auto utf8 = path.filename().u8string();
        return std::string(utf8.begin(), utf8.end()); 
    }
    std::string extension() const { 
        const auto utf8 = path.extension().u8string();
        return std::string(utf8.begin(), utf8.end()); 
    }
};

struct ScanResult {
    std::vector<FileInfo> files;
    std::vector<ScanError> errors;

    bool has_errors() const { return !errors.empty(); }
};
