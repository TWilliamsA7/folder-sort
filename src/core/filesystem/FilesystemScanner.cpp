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

    // Use directory_options to handle basic permission issues at the OS level
    std::filesystem::directory_options dir_options = 
        std::filesystem::directory_options::skip_permission_denied;

    if (options_.follow_symlinks) {
        dir_options |= std::filesystem::directory_options::follow_directory_symlink;
    }

    // Initialize iterator
    auto it = std::filesystem::recursive_directory_iterator(root_, dir_options, ec);
    
    if (ec) {
        result.errors.push_back({root_, map_error(ec), ec});
        return result;
    }

    auto end = std::filesystem::recursive_directory_iterator();

    while (it != end) {
        const auto& path = it->path();
        
        // 1. Check for Hidden Files (Windows specific)
        if (!options_.include_hidden && is_hidden(path)) {
            if (it->is_directory()) {
                it.disable_recursion_pending();
            }
            it.increment(ec); // Manually increment to skip
            continue;
        }

        // 2. Determine Type
        FileType type = get_file_type(*it);

        // 3. Depth Control
        if (options_.max_depth >= 0 && it.depth() > options_.max_depth) {
            if (type == FileType::Directory) {
                it.disable_recursion_pending();
            }
            it.increment(ec);
            continue;
        }

        // 4. Decide if we should record this entry
        bool should_record = true;
        if (type == FileType::Directory && !options_.include_directories) {
            should_record = false;
        }

        if (should_record) {
            FileInfo info;
            info.path = path;
            info.type = type;

            if (type == FileType::RegularFile) {
                std::error_code size_ec;
                auto size = std::filesystem::file_size(path, size_ec);
                if (!size_ec) info.size = size;
            }
            
            // Add last modified time
            std::error_code time_ec;
            auto ftime = std::filesystem::last_write_time(path, time_ec);
            if (!time_ec) info.last_modified = ftime;

            result.files.push_back(std::move(info));
        }

        // 5. Safe Increment
        it.increment(ec);
        if (ec) {
            result.errors.push_back({path, map_error(ec), ec});
            ec.clear();
        }
    }

    return result;
}