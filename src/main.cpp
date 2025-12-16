#include "folder_sort/scan.hpp"
#include <iostream>

int main(int argc, char** argv) {
    const char* path = (argc > 1) ? argv[1] : ".";
    int rc = folder_sort::scan_directory(path);
    if (rc != 0) {
        std::cerr << "scan_directory failed with code " << rc << "\n";
        return rc;
    }
    std::cout << "folder-sort completed successfully.\n";
    return 0;
}
