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
    if (ec == errc::filename_too_long) {
        return ScanErrorType::PathTooLong;
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

    // Use directory_options to handle basic permission issues at the OS level
    std::filesystem::directory_options dir_options = 
        std::filesystem::directory_options::skip_permission_denied;

    if (options_.follow_symlinks) {
        dir_options |= std::filesystem::directory_options::follow_directory_symlink;
    }

    std::filesystem::recursive_directory_iterator it(root_, dir_options, ec);
    std::filesystem::recursive_directory_iterator end;
    
    if (ec) {
        result.errors.push_back({root_, map_error(ec), ec});
        return result;
    }

    for (; it != end; ) {
        const auto& path = it->path();
        
        // Depth Control
        if (options_.max_depth >= 0 && it.depth() > options_.max_depth) {
            if (it->is_directory()) {
                it.disable_recursion_pending();
            }
            it.increment(ec);
            continue;
        }

        // File Type
        FileType type = get_file_type(*it);

        // Symlink policy
        if (type == FileType::Symlink && !options_.follow_symlinks) {
            result.errors.push_back({
                path,
                ScanErrorType::ReparsePoint,
                {}
            });
            it.disable_recursion_pending();
            it.increment(ec);
            continue;
        }

        // Hidden
        if (!options_.include_hidden && is_hidden(path)) {
            if (it->is_directory()) {
                it.disable_recursion_pending();
            }
            it.increment(ec);
            continue;
        }

        // Record Entry
        bool should_record = true;
        if (type == FileType::Directory && !options_.include_directories) {
            should_record = false;
        }

        if (should_record) {
            FileInfo info;
            info.type = type;

            // Path normalization
            if (options_.normalize_paths) {
                std::error_code norm_ec;
                info.path = std::filesystem::weakly_canonical(path, norm_ec);
                if (norm_ec) {
                    info.path = path;
                }
            } else {
                info.path = path;
            }

            if (type == FileType::RegularFile) {
                std::error_code size_ec;
                info.size = std::filesystem::file_size(path, size_ec);
                if (size_ec) {
                    result.errors.push_back({
                        path,
                        map_error(size_ec),
                        size_ec
                    });

                    if (!options_.allow_permission_errors)
                        return result;
                }
            }
            
            // Last time modified
            std::error_code time_ec;
            info.last_modified = std::filesystem::last_write_time(path, time_ec);
            if (time_ec) {
                result.errors.push_back({
                    path,
                    map_error(time_ec),
                    time_ec
                });

                if (!options_.allow_permission_errors)
                    return result;
            }

            result.files.push_back(std::move(info));
        }

        // Increment
        it.increment(ec);
        if (ec) {
            result.errors.push_back({ path, map_error(ec), ec });
            if (!options_.allow_permission_errors)
                return result;
            ec.clear();
        }
    }

    return result;
}