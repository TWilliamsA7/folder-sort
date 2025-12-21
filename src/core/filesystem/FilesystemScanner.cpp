// src/core/filesystem/FilesystemScanner.cpp

#include "core/filesystem/FilesystemScanner.hpp"

FilesystemScanner::FilesystemScanner (
    std::filesystem::path root,
    ScanOptions options
) : options_(options) {
    std::error_code ec;

    // Normalize path
    auto canonical = std::filesystem::weakly_canonical(root, ec);
    if (ec) {
        throw std::runtime_error("Failed to canonicalize root path");
    }

    if (!std::filesystem::exists(canonical)) {
        throw std::runtime_error("Root path does not exist");
    }

    if (!std::filesystem::is_directory(canonical)) {
        throw std::runtime_error("Root path is not a directory");
    }

    root_ = canonical;
}


ScanErrorType map_error(const std::error_code& ec) {
    using std::errc;

    if (ec == errc::permission_denied) {
        return ScanErrorType::PermissionDenied;
    }
    if (ec == errc::no_such_file_or_directory) {
        return ScanErrorType::NotFound;
    }

    return ScanErrorType::Unknown;
}

static bool is_hidden(const std::filesystem::path& path) {
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_HIDDEN) != 0;
}

static FileType get_file_type(const std::filesystem::directory_entry& entry) {
    std::error_code ec;
    auto status = entry.symlink_status(ec);
    if (ec) return FileType::Other;

    if (std::filesystem::is_symlink(status))
        return FileType::Symlink;
    if (std::filesystem::is_regular_file(status))
        return FileType::RegularFile;
    if (std::filesystem::is_directory(status))
        return FileType::Directory;

    return FileType::Other;
}

ScanResult FilesystemScanner::scan() {
    ScanResult result;

    std::error_code ec;

    std::filesystem::directory_options dir_options =
        std::filesystem::directory_options::skip_permission_denied;

    if (options_.follow_symlinks) {
        dir_options |= std::filesystem::directory_options::follow_directory_symlink;
    }

    std::filesystem::recursive_directory_iterator it(root_, dir_options, ec);
    std::filesystem::recursive_directory_iterator end;

    if (ec) {
        result.errors.push_back({
            root_,
            map_error(ec),
            ec
        });
        return result;
    }

    for (; it != end; it.increment(ec)) {
        if (ec) {
            result.errors.push_back({
                it->path(),
                map_error(ec),
                ec
            });

            ec.clear();
            continue;
        }

        
        const auto& path = it->path();
        
        // Hidden file behavior
        if (!options_.include_hidden && is_hidden(path)) {
            it.disable_recursion_pending();
            continue;
        }

        FileType type = get_file_type(*it);

        if (type == FileType::Symlink && !options_.follow_symlinks) {
            it.disable_recursion_pending();
            continue;
        }

        // Depth control
        if (it.depth() > static_cast<int>(options_.max_depth)) {
            it.disable_recursion_pending();
            continue;
        }

        FileInfo info;
        info.path = path;
        info.type = type;

        if (type == FileType::RegularFile) {
            std::error_code size_ec;
            auto size = std::filesystem::file_size(path, size_ec);
            if (!size_ec) info.size = size;

            std::error_code time_ec;
            auto time = std::filesystem::last_write_time(path, time_ec);
            if (!time_ec) info.last_modified = time;
        }

        result.files.push_back(std::move(info));
    }

    return result;
}
