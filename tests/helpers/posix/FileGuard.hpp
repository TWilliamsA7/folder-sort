// tests/helpers/posix/FileGuard.hpp

#ifndef _WIN32

#pragma once

#include <filesystem>
#include <sys/stat.h>

struct PosixPermGuard {
    std::filesystem::path p;
    explicit PosixPermGuard(const std::filesystem::path& path) : p(path) {
        chmod(p.c_str(), 0000);
    }
    ~PosixPermGuard() {
        chmod(p.c_str(), 0755);
    }
};

#endif
