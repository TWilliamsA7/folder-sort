// include/core/rules/Rule.hpp

#include <memory>
#include <vector>
#include "core/conditions/Condition.hpp"
#include "core/actions/Action.hpp"

// Defines a set of conditions that will trigger a set of actions
// Immutable after Creation
// For actions to be applied, a file must meet all of the expressed conditions
class Rule {
    public:
        Rule(std::string name, std::vector<std::unique_ptr<Condition>> conditions, std::vector<std::unique_ptr<Action>> actions);

        // * Returns set of actions to apply to a given file depending on conditions
        std::vector<std::unique_ptr<Action>> apply(const FileInfo&) const;

        const std::string name() const;
        const std::vector<std::unique_ptr<Condition>> conditions() const;
        const std::vector<std::unique_ptr<Action>> actions() const;

    private:
        std::string name_;
        std::vector<std::unique_ptr<Condition>> conditions_;
        std::vector<std::unique_ptr<Action>> actions_; 

};