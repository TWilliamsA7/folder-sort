// src/core/conditions/NameCondition.cpp

#include "core/conditions/NameCondition.hpp"

NameCondition::NameCondition(std::regex pattern) : pattern_(pattern) {}

bool NameCondition::check(const FileInfo& file) const {
    std::smatch matches;
    const auto utf8 = file.path.stem().u8string();
    const std::string name = std::string(utf8.begin(), utf8.end());
    return std::regex_match(name, matches, pattern_);
}

const std::regex& NameCondition::pattern () const { return pattern_; }