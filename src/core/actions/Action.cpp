// src/core/actions/Action.cpp

#include "core/actions/Action.hpp"

Action::Action(FileInfo file) : file_(file) {}

const FileInfo& Action::file() const {
    return file_;
}