// src/core/rules/ExtensionRule.cpp

#include "core/rules/ExtensionRule.hpp"
#include "core/actions/MoveAction.hpp"

#include <algorithm>
#include <cctype>

ExtensionRule::ExtensionRule(std::string extension, std::filesystem::path destination) : extension_(std::move(extension)), destination_(std::move(destination)) {
    // Ensure extension starts with '.'
    if (!extension_.empty() && extension_[0] != '.') {
        extension_.insert(extension_.begin(), '.');
    }
    // Ensure extension is in lowercase
    std::transform(extension_.begin(), extension_.end(), extension_.begin(), [](unsigned char c){ return std::tolower(c); });
}

std::vector<std::unique_ptr<Action>> ExtensionRule::apply(const FileInfo& file) const {
    std::vector<std::unique_ptr<Action>> actions;

    // If the file does not match return empty vector
    if (file.extension() != extension_) {
        return actions;
    }

    // If file extension matches, append MoveAction
    actions.push_back(
        std::make_unique<MoveAction>(file, destination_)
    );

    return actions;
}

const std::string& ExtensionRule::extension() const { return extension_; }
const std::filesystem::path& ExtensionRule::destination() const { return destination_; }