// src/core/filesystem/platform/FilesystemPlatform_win.cpp

#include "core/filesystem/platform/FilesystemPlatform.hpp"
#include <windows.h>

bool fs_platform::is_hidden(const std::filesystem::path& p) {
    DWORD attrs = GetFileAttributesW(p.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES) &&
           (attrs & FILE_ATTRIBUTE_HIDDEN);
}
