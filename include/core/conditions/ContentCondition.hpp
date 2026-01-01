// include/core/conditions/ContentCondition.hpp

#pragma once

#include "Condition.hpp"

#include <regex>

class ContentCondition : public Condition {

    public:
        ContentCondition(std::regex pattern);
        bool check(const FileInfo&) const override;

        const std::regex& pattern() const;

    private:
        std::regex pattern_;
};