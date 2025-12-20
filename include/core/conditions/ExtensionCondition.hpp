// include/core/rules/ExtensionCondition.hpp

#pragma once

#include "Condition.hpp"

// * Apply move actions to files given a specified extension
class ExtensionCondition : public Condition {

    public:
        ExtensionCondition(std::string extension);

        bool check(const FileInfo&) const override; 

        // * Extension is in the form ".***"
        const std::string& extension() const;

    private:
        // * Extension is in the form ".***"
        std::string extension_;
};