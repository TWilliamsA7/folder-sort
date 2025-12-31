// include/core/conditions/NameCondition.hpp

#pragma once

#include "Condition.hpp"

#include <regex>

// * Apply move actions to files given a specified extension
class NameCondition : public Condition {

    public:
        NameCondition(std::regex pattern);
        bool check(const FileInfo&) const override; 

        const std::regex& pattern() const;

    private:
        std::regex pattern_;
};