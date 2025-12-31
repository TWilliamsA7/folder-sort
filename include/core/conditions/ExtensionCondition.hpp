// include/core/conditions/ExtensionCondition.hpp

#pragma once

#include "Condition.hpp"

// * Apply move actions to files given a specified extension
class ExtensionCondition : public Condition {

    public:
        ExtensionCondition(std::vector<std::string> extensions);

        bool check(const FileInfo&) const override; 

        // * Extension is in the form ".***"
        const std::vector<std::string>& extensions() const;

    private:
        // * Extension is in the form ".***"
        std::vector<std::string> extensions_;
};