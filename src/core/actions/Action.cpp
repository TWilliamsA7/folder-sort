// src/core/actions/Action.cpp

#include "core/actions/Action.hpp"

Action::Action() {}
Action::Action(FileInfo* file) : file_(file) {}

const void Action::setFile(FileInfo* file) {
    file_ = std::move(file);
}

const FileInfo* Action::getFile() const {
    return file_;
}