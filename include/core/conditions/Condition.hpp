// include/core/conditions/Condition.hpp

#pragma once

#include <vector>
#include <memory>
#include "core/actions/Action.hpp"
#include "core/model/FileInfo.hpp"

class Condition {
    public:
        virtual ~Condition() = default;
        virtual bool check(const FileInfo&) const = 0;
};