// include/core/rules/Rule.hpp

#include <memory>
#include <vector>
#include "core/conditions/Condition.hpp"
#include "core/actions/Action.hpp"

// Defines a set of conditions that will trigger a set of actions
// Immutable after Creation
class Rule {
    public:
        Rule(std::string name, std::vector<std::unique_ptr<Condition>> conditions, std::vector<std::unique_ptr<Action>> actions);

        const std::string name() const;
        const std::vector<std::unique_ptr<Condition>> conditions() const;
        const std::vector<std::unique_ptr<Action>> actions() const;

    private:
        std::string name_;
        std::vector<std::unique_ptr<Condition>> conditions_;
        std::vector<std::unique_ptr<Action>> actions_; 

};