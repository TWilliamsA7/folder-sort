// src/core/conditions/ContentCondition.cpp

#include "core/conditions/ContentCondition.hpp"

ContentCondition::ContentCondition(std::regex pattern) : pattern_(pattern) {}

bool ContentCondition::check(const FileInfo& file) const {
    std::ifstream fileStream(file.path, std::ios::binary);
    if (!fileStream) return false;

    if (!shouldRunCheck(file, fileStream)) {
        return false;
    }

    fileStream.clear();
    fileStream.seekg(0);

    std::string line;
    while (std::getline(fileStream, line)) {
        if (std::regex_search(line, pattern_)) {
            return true;
        }
    }

    return false;
}

const std::regex& ContentCondition::pattern() const { return pattern_; }

bool ContentCondition::shouldRunCheck(const FileInfo& file, std::ifstream& fileStream) {
    // Check against the file size first
    if (file.size.has_value() && file.size.value() > MAX_TEXT_SCAN_SIZE) {
        return false;
    }

    // Check against extension
    if (TEXT_EXTENSIONS.find(file.extension()) == TEXT_EXTENSIONS.end()) {
        return false;
    }

    // Check by reading portion of file
    return isTextLike(fileStream);
}

bool ContentCondition::isTextLike(std::ifstream& fileStream) {
    constexpr size_t SAMPLE_SIZE = 4096;
    char buffer[SAMPLE_SIZE];

    fileStream.read(buffer, SAMPLE_SIZE);
    std::streamsize bytesRead = fileStream.gcount();

    size_t nonPrintable = 0;
    for (int i = 0; i < bytesRead; ++i) {
        unsigned char c = buffer[i];
        if (c == '\0') return false;
        if (c < 9 || (c > 13 && c < 32)) nonPrintable++;
    }

    return (nonPrintable < bytesRead * 0.1); // 10% threshold
}
