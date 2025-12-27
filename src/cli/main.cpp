// src/cli/main.cpp

#include <iostream>
#include <CLI/CLI.hpp>
#include "core/app/Application.hpp"

int run_cli(int argc, char** argv);

int main(int argc, char** argv) {
    try {
        return run_cli(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}

int run_cli(int argc, char** argv) {
    CLI::App app{"File Managment Tool"};

    AppConfig config;

    app.add_option("-r,--root", config.root_dir, "Root directory to scan")
        ->required()
        ->check(CLI::ExistingDirectory);

    app.add_option("-c,--config", config.rules_file, "Path to Rules YAML file")
        ->required()
        ->check(CLI::ExistingFile);

    auto verbose = app.add_flag("-v,--verbose", config.verbose, "Enable Logging");
    app.add_flag("--dry-run", config.dry_run, "Do not execute actions")
        ->needs(verbose);

    app.add_option("-l,--log", config.log_dir, "Log directory")
        ->needs(verbose)
        ->check(CLI::ExistingDirectory);

    app.add_flag("-s,--symlink", config.follow_symlinks, "Enable Symlink Following");
    app.add_flag("-h,--hidden", config.include_hidden, "Enable Inclusion of Hidden Elements");
    app.add_flag("-p,--permission", config.allow_permission_errors, "Allow permissions errors during folder scan");
    app.add_flag("-d,--dir", config.include_directories, "Enable inclusion of directories in scan");
    app.add_flag("-a,--absolute", config.normalize_paths, "Disable normalization of paths");
    app.add_option("--max-depth", config.max_depth, "Specifies Maximum Depth of Search | -1 means no maximum");


    CLI11_PARSE(app, argc, argv);

    Application application(config);
    return application.run();
}
