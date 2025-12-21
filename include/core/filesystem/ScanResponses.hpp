// include/core/filesystem/ScanResponses.hpp

#include <filesystem>
#include <optional>
#include <string>

enum class ScanErrorType {
    PermissionDenied,
    NotFound,
    SymlinkLoop,
    Unknown
};

struct ScanError {
    std::filesystem::path path;
    ScanErrorType type;
    std::error_code ec;
};

struct FileInfo {
    std::filesystem::path path;
    std::optional<std::uintmax_t> size;
    std::optional<std::filesystem::file_time_type> last_modified;

    std::string filename() const { return path.filename().string(); }
    std::string extension() const { return path.extension().string(); }
};

struct ScanResult {
    std::vector<FileInfo> files;
    std::vector<ScanError> errors;
};