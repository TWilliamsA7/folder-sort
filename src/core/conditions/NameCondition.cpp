// src/core/conditions/NameCondition.cpp

#include "core/conditions/NameCondition.hpp"

NameCondition::NameCondition(std::regex pattern) : pattern_(pattern) {}

bool NameCondition::check(const FileInfo& file) const {
    std::smatch matches;
    const std::string name = file.path.stem().string();
    return std::regex_match(name, matches, pattern_);
}

const std::regex& NameCondition::pattern () const { return pattern_; }