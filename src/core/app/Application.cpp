// src/core/app/Application.cpp

#include "core/app/Application.hpp"

Application::Application(AppConfig config) : config_(config) {}


int Application::run() {
    const auto logFile = logging::Logger::Init(config_.log_dir, config_.verbose);

    auto log = logging::Logger::Get("app");

    ConfigLoader rulesLoader(config_.rules_file.string());
    auto rule_yaml = rulesLoader.load();

    auto rules = RuleFactory::buildRules(rule_yaml);
    RuleEngine engine;
    for (auto& rule : rules) {
        engine.addRule(std::move(rule));
    }

    ScanOptions sc_options;

    sc_options.follow_symlinks = config_.follow_symlinks;
    sc_options.include_hidden = config_.include_hidden;
    sc_options.allow_permission_errors = config_.allow_permission_errors;
    sc_options.max_depth = config_.max_depth;
    sc_options.include_directories = config_.include_directories;
    sc_options.normalize_paths = config_.normalize_paths;

    FilesystemScanner scanner(config_.root_dir, sc_options);
    auto res = scanner.scan();



    ActionExecutor executor(config_.dry_run, config_.verbose);

    log->info("Starting rule evaluation and execution");
    for (auto file : res.files) {
        auto actions = engine.evaluate(file, config_.root_dir);
        executor.execute(std::move(actions), file);
    }

    engine.logRuleStats();

    log->info("Completed Execution");
    logging::Logger::Shutdown();

    return 0;
}