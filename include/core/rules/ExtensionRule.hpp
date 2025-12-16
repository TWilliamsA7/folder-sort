// include/core/rules/ExtensionRule.hpp

#pragma once

#include "Rule.hpp"

class ExtensionRule : public Rule {

    public:
        ExtensionRule(std::string extension, std::filesystem::path destination);

        std::vector<std::unique_ptr<Action>> apply (const FileInfo&) const override;  
        const std::string& extension() const;
        const std::filesystem::path& destination() const;

    private:
        std::string extension_;
        std::filesystem::path destination_;
};