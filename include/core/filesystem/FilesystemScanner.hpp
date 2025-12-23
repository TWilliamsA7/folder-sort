// include/core/filesystem/FilesystemScanner.hpp

#pragma once

#include <vector>
#include <filesystem>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include "core/filesystem/ScanResponses.hpp"
#include "core/filesystem/platform/FilesystemPlatform.hpp"

struct ScanOptions {
    bool follow_symlinks = false;
    bool include_hidden = false;
    bool allow_permission_errors = true;
    int max_depth = -1;

    bool include_directories = false;
    bool normalize_paths = true;
    bool logging = false;
};

class FilesystemScanner {
    public: 
        explicit FilesystemScanner(
            std::filesystem::path root,
            ScanOptions options = {}
        );
        
        ScanResult scan();


    private:
        std::filesystem::path root_;
        ScanOptions options_;

};

