// src/core/conditions/ExtensionCondition.cpp

#include "core/conditions/ExtensionCondition.hpp"
#include <algorithm>
#include <cctype>

ExtensionCondition::ExtensionCondition(std::string extension) : extension_(std::move(extension)) {
    // Ensure extension starts with '.'
    if (!extension_.empty() && extension_[0] != '.') {
        extension_.insert(extension_.begin(), '.');
    }
    // Ensure extension is in lowercase
    std::transform(extension_.begin(), extension_.end(), extension_.begin(), [](unsigned char c){ return std::tolower(c); });
}

bool ExtensionCondition::check(const FileInfo& file) const {
    return file.extension() == extension_;
}

const std::string& ExtensionCondition::extension() const { return extension_; }
