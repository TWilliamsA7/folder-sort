// include/core/actions/Action.hpp

#pragma once

#include <string>
#include <filesystem>
#include "core/filesystem/ScanResponses.hpp"
#include "core/actions/ActionSpec.hpp"

class Action {
    public:
        virtual ~Action() = default;

        virtual ActionType type() const = 0;
        virtual const std::string describe() const = 0;
        virtual const std::string describe(const FileInfo& file) const = 0;
        virtual void execute(const FileInfo& file) const = 0;
};
