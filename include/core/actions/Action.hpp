// include/core/actions/Action.hpp

#pragma once

#include <string>
#include "core/model/FileInfo.hpp"
#include "core/actions/ActionSpec.hpp"

class Action {
    public:
        virtual ~Action() = default;

        virtual ActionType type() const = 0;
        virtual std::string describe() const = 0;
        virtual void execute(FileInfo& file) const = 0;
};
