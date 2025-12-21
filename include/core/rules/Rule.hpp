// include/core/rules/Rule.hpp

#pragma once

#include <memory>
#include <vector>
#include "core/conditions/Condition.hpp"
#include "core/actions/ActionSpec.hpp"

// Defines a set of conditions that will trigger a set of actions
// Immutable after Creation
// For actions to be applied, a file must meet all of the expressed conditions
class Rule {
    public:
        Rule(
            std::string name, 
            std::vector<std::unique_ptr<Condition>> conditions, 
            std::vector<ActionSpec> actions
        );

        bool matches(const FileInfo& file) const;

        const std::string name() const;
        const std::vector<std::unique_ptr<Condition>> conditions() const;
        const std::vector<ActionSpec>& actions() const;

    private:
        std::string name_;
        std::vector<std::unique_ptr<Condition>> conditions_;
        std::vector<ActionSpec> actions_; 

};