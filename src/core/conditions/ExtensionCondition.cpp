// src/core/conditions/ExtensionCondition.cpp

#include "core/conditions/ExtensionCondition.hpp"
#include <algorithm>
#include <cctype>

ExtensionCondition::ExtensionCondition(std::vector<std::string> extensions) {
    for (std::string& extension : extensions) {
        if (!extension.empty() && extension[0] != '.') {
            extension.insert(extension.begin(), '.');
        }
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c){ return std::tolower(c); });
    }

    extensions_ = extensions;
}

bool ExtensionCondition::check(const FileInfo& file) const {

    for (std::string extension : extensions_) {
        if (file.extension() == extension)
            return true;
    }
    return false;
}

const std::vector<std::string>& ExtensionCondition::extensions() const { return extensions_; }
