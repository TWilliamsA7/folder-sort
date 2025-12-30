// include/core/rules/RuleEngine.hpp

#pragma once

#include <memory>
#include <vector>
#include "Rule.hpp"


class RuleEngine {
    public: 
        void addRule(std::unique_ptr<Rule>);
        std::vector<std::unique_ptr<Action>> evaluate(const FileInfo&, const std::filesystem::path&) const;

    private:
        std::vector<std::unique_ptr<Rule>> rules_;

};