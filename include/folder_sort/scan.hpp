#pragma once

#include <string>

namespace folder_sort {

    // Returns 0 on success, non-zero on error.
    int scan_directory(const std::string& path);

}