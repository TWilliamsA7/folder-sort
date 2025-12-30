// src/core/rules/RuleEngine.cpp

#include "core/rules/RuleEngine.hpp"
#include "core/actions/ActionFactory.hpp"

namespace {
    const char* kLoggerName = "app.rules.engine";
};

RuleEngine::RuleEngine() {
    auto log = logging::Logger::Get(kLoggerName);
    log->info("Initialized Rules Engine");
}

void RuleEngine::addRule(std::unique_ptr<Rule> rule) {
    rules_.push_back(std::move(rule));
}

std::vector<std::unique_ptr<Action>> RuleEngine::evaluate(const FileInfo& file, const std::filesystem::path& root_dir) const {

    std::vector<std::unique_ptr<Action>> result;

    for (const auto& rule : rules_) {
        if (!rule->matches(file)) continue;

        
        for (const auto& spec : rule->actions()) {
            result.push_back(ActionFactory::create(spec, root_dir, rule->files_touched));
        }

        rule->files_touched++;
    }
    
    return result;
}

void RuleEngine::logRuleStats(void) const {
    auto log = logging::Logger::Get(kLoggerName);
    for (const auto& rule : rules_) {
        log->info("Rule {} acted on {} files", rule->name(), rule->files_touched);
    }
}