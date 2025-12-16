// include/core/rules/Rule.hpp

#pragma once

#include <vector>
#include <memory>
#include "../actions/Action.hpp"
#include "../model/FileInfo.hpp"

class Rule {
    public:
        virtual ~Rule() = default;
        virtual std::vector<std::unique_ptr<Action>> apply(const FileInfo&) const = 0;

};