#include "folder_sort/scan.hpp"
#include "core/model/FileInfo.h"
#include <iostream>

int main(int argc, char** argv) {
    const char* path = (argc > 1) ? argv[1] : ".";
    int rc = folder_sort::scan_directory(path);

    FileInfo ftest;
    ftest.name = "test.jpg";

    std::cout << ftest.name << std::endl;

    if (rc != 0) {
        std::cerr << "scan_directory failed with code " << rc << "\n";
        return rc;
    }
    std::cout << "folder-sort completed successfully.\n";
    return 0;
}
