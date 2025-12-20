// include/core/actions/Action.hpp

#pragma once

#include <string>
#include "core/model/FileInfo.hpp"

enum class ActionType {
    MOVE,
    RENAME,
    DELETE
};

class Action {

    public:
        Action();
        Action(FileInfo* file);

        virtual ~Action() = default;
        virtual ActionType type() const = 0;
        virtual std::string describe() const = 0;
        const void setFile(FileInfo* file);
        const FileInfo* getFile() const;

    private: 
        FileInfo* file_ = nullptr;
};