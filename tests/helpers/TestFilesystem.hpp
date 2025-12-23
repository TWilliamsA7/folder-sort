// tests/helpers/TestFilesystem.hpp

#include <filesystem>
#include <string>
#include <fstream>

struct TestTree {
    std::filesystem::path root;

    explicit TestTree(const std::string& name) {
        root = std::filesystem::temp_directory_path() / name;
        std::filesystem::create_directories(root);
    }

    ~TestTree() {
        std::filesystem::remove_all(root);
    }

    std::filesystem::path file(const std::string& name) {
        auto p = root / name;
        std::ofstream(p.string()) << "x";
        return p;
    }

    std::filesystem::path dir(const std::string& name) {
        auto p = root / name;
        std::filesystem::create_directories(p);
        return p;
    }
};
