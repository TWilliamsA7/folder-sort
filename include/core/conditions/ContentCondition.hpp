// include/core/conditions/ContentCondition.hpp

#pragma once

#include "Condition.hpp"

#include <regex>
#include <unordered_set>
#include <string>
#include <fstream>

class ContentCondition : public Condition {

    public:
        ContentCondition(std::regex pattern);
        bool check(const FileInfo&) const override;

        const std::regex& pattern() const;

    private:
        // Helper function for determining if a file is text beyond its extension
        static bool shouldRunCheck(const FileInfo& file, std::ifstream& fileStream);
        static bool isTextLike(std::ifstream& fileStream);
        std::regex pattern_;
};

constexpr std::uintmax_t MAX_TEXT_SCAN_SIZE = 5 * 1024 * 1024; // 5MB

const std::unordered_set<std::string> TEXT_EXTENSIONS = {
    ".txt", ".log", ".md", ".yaml", ".yml",
    ".json", ".xml", ".csv", ".ini", ".cfg"
};
