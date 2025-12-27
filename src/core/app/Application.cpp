// src/core/app/Application.cpp

#include "core/app/Application.hpp"

Application::Application(AppConfig config) : config_(config) {}


int Application::run() {

    if (config_.verbose) {
        const auto logFile = logging::Logger::Init(config_.log_dir);
    }

    auto log = config_.verbose ? logging::Logger::Get("app") : nullptr;

    ConfigLoader rulesLoader(config_.rules_file.string());
    auto rule_yaml = rulesLoader.load();
    if (log) log->info("Loaded Rules from {}", config_.rules_file.string());

    auto rules = RuleFactory::buildRules(rule_yaml);
    RuleEngine engine;
    for (auto& rule : rules) {
        engine.addRule(std::move(rule));
    }
    if (log) log->info("Configured {} Rules", rules.size());

    ScanOptions sc_options;

    sc_options.logging = config_.verbose;
    sc_options.follow_symlinks = config_.follow_symlinks;
    sc_options.include_hidden = config_.include_hidden;
    sc_options.allow_permission_errors = config_.allow_permission_errors;
    sc_options.max_depth = config_.max_depth;
    sc_options.include_directories = config_.include_directories;
    sc_options.normalize_paths = config_.normalize_paths;

    FilesystemScanner scanner(config_.root_dir, sc_options);
    auto res = scanner.scan();

    if (res.has_errors()) {
        if (log) log->warn("{} errors occurred during the file scan", res.errors.size());
    }

    ActionExecutor executor(config_.dry_run, config_.verbose);

    if (log) log->info("Starting rule evaluation and execution");
    for (auto file : res.files) {
        auto actions = engine.evaluate(file);
        executor.execute(std::move(actions), file);
    }

    logging::Logger::Shutdown();

    return 0;
}