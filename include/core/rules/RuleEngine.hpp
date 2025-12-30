// include/core/rules/RuleEngine.hpp

#pragma once

#include <memory>
#include <vector>
#include "Rule.hpp"
#include "logging/Logger.hpp"


class RuleEngine {
    public: 
        RuleEngine();
        void addRule(std::unique_ptr<Rule>);
        std::vector<std::unique_ptr<Action>> evaluate(const FileInfo&, const std::filesystem::path&) const;
        void logRuleStats(void) const;

    private:
        std::vector<std::unique_ptr<Rule>> rules_;

};