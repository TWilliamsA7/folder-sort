// include/core/rules/ExtensionRule.hpp

#pragma once

#include "Rule.hpp"

// * Apply move actions to files given a specified extension
class ExtensionRule : public Rule {

    public:
        ExtensionRule(std::string extension, std::filesystem::path destination);

        std::vector<std::unique_ptr<Action>> apply (const FileInfo&) const override; 

        // * Extension is in the form ".***"
        const std::string& extension() const;
        const std::filesystem::path& destination() const;

    private:
        // * Extension is in the form ".***"
        std::string extension_;
        std::filesystem::path destination_;
};