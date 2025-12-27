// include/core/app/Application.hpp

#pragma once

#include <filesystem>

#include "logging/Logger.hpp"
#include "core/filesystem/FilesystemScanner.hpp"
#include "config/RuleFactory.hpp"
#include "config/ConfigLoader.hpp"
#include "core/rules/RuleEngine.hpp"
#include "core/actions/ActionExecutor.hpp"


struct AppConfig {
    std::filesystem::path root_dir;
    std::filesystem::path log_dir;
    std::filesystem::path rules_file;
    bool dry_run{false};
    bool verbose{false}; // logging

    // Scanner options
    bool follow_symlinks = false;
    bool include_hidden = false;
    bool allow_permission_errors = true;
    int max_depth = -1;

    bool include_directories = false;
    bool normalize_paths = true;

};

class Application {
    public:
        explicit Application(AppConfig config);
        int run();

    private:
        AppConfig config_;
};