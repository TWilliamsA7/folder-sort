// src/core/filesystem/FilesystemScanner.cpp

#include "core/filesystem/FilesystemScanner.hpp"

namespace {
    const char* kLoggerName = "app.filesystem.scanner";
}

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


    auto log = logging::Logger::Get(kLoggerName);
    log->info("Initialized Filesystem Scanner on root: {}", canonical.string());

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
    
    auto log = logging::Logger::Get(kLoggerName);

    // Use directory_options to handle basic permission issues at the OS level
    std::filesystem::directory_options dir_options = std::filesystem::directory_options::none;

    if (options_.follow_symlinks) {
        dir_options |= std::filesystem::directory_options::follow_directory_symlink;
    }

    std::filesystem::recursive_directory_iterator it(root_, dir_options, ec);
    std::filesystem::recursive_directory_iterator end;
    
    if (ec) {
        log->error("Failed to initialize directory pointer: {}", ec.message());
        result.errors.push_back({root_, map_error(ec), ec});
        return result;
    }

    while (it != end) {
        std::filesystem::directory_entry entry;

        // Snapshot Entry
        try {
            entry = *it;
        } catch (...) {
            log->error("Failed to open filesystem entry due to dereference of invalid pointer");
            result.errors.push_back({ root_, ScanErrorType::Unknown, {} });
            break;
        }

        const auto current_path = entry.path();
        bool skip_content = false;

        // Depth control
        if (options_.max_depth >= 0 && it.depth() > options_.max_depth) {
            log->info("Skipped Entry {} | Exceeded Maximum Scan Depth", current_path.string());
            if (entry.is_directory()) {
                it.disable_recursion_pending();
            }
            skip_content = true;
        }

        // STEP 3: File type
        FileType type = get_file_type(entry);

        // Symlink policy
        if (!skip_content && type == FileType::Symlink && !options_.follow_symlinks) {
            log->warn("Skipped Entry {} | Encountered Symlink", current_path.string());
            result.errors.push_back({
                current_path,
                ScanErrorType::ReparsePoint,
                {}
            });
            it.disable_recursion_pending();
            skip_content = true;
        }

        // Hidden files
        if (!skip_content && !options_.include_hidden && fs_platform::is_hidden(current_path)) {
            log->info("Skipped Entry {} | Entry is Hidden", current_path.string());
            if (entry.is_directory()) {
                it.disable_recursion_pending();
            }
           skip_content = true;
        }

        // Record Entry
        bool should_record = !(type == FileType::Directory && !options_.include_directories);

        if (!skip_content && should_record) {
            FileInfo info;
            info.type = type;
            info.path = options_.normalize_paths
                ? std::filesystem::weakly_canonical(current_path)
                : current_path;

            if (type == FileType::RegularFile) {
                std::error_code size_ec;
                info.size = std::filesystem::file_size(current_path, size_ec);
                if (size_ec) {
                    log->warn("Entry {} | Failed to read size | {}", current_path.string(), size_ec.message());
                    result.errors.push_back({ current_path, map_error(size_ec), size_ec });
                    if (!options_.allow_permission_errors)
                        return result;
                }
            }

            std::error_code time_ec;
            info.last_modified = std::filesystem::last_write_time(current_path, time_ec);
            if (time_ec) {
                log->warn("Entry {} | Failed to last write time | {}", current_path.string(), time_ec.message());
                result.errors.push_back({ current_path, map_error(time_ec), time_ec });
                if (!options_.allow_permission_errors)
                    return result;
            }

            log->info("Scanned Entry {}", current_path.string());
            result.files.push_back(std::move(info));
        }

        // STEP 7: advance
        it.increment(ec);
        if (ec) {
            log->error("Error occurred during traversal: {}", ec.message());
            result.errors.push_back({ current_path, map_error(ec), ec });
            if (!options_.allow_permission_errors)
                return result;
            ec.clear();
        }
    }

    if (result.has_errors()) {
        log->warn("{} errors occurred during the file scan", result.errors.size());
    }

    log->info("Completed Scan");
    return result;
}