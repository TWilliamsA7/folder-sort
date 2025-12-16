// include/core/actions/Action.hpp

#pragma once

#include <string>
#include "../model/FileInfo.hpp"

enum class ActionType {
    MOVE,
    RENAME,
    DELETE
};

class Action {

    public:
        virtual ~Action() = default;
        virtual ActionType type() const = 0;
        virtual std::string describe() const = 0;
        const FileInfo& file() const;

    protected: 
        FileInfo file_;
};