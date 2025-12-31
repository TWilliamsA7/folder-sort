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
        void addCatchAllAction(ActionSpec);
        std::vector<std::unique_ptr<Action>> evaluate(const FileInfo&, const std::filesystem::path&) const;
        void logRuleStats(void) const;

    private:
        std::vector<std::unique_ptr<Rule>> rules_;
        std::vector<ActionSpec> catch_all_actions_;
        mutable int unmatched_files_ = 0;

};