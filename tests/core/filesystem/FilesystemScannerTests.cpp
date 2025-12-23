// tests/core/filesystem/FilesystemScannerTests.cpp

#include <gtest/gtest.h>
#include <fstream>

#include "core/filesystem/FilesystemScanner.hpp"

TEST(FilesystemScannerTests, ConstructorAcceptsValidDirectory) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_valid";
    std::filesystem::create_directories(temp);

    EXPECT_NO_THROW({
        FilesystemScanner scanner(temp);
    });

    std::filesystem::remove_all(temp);
}


TEST(FilesystemScannerTests, ConstructorRejectsNonExistentPath) {
    auto fake = std::filesystem::temp_directory_path() / "does_not_exist";

    EXPECT_THROW(
        FilesystemScanner scanner(fake),
        std::runtime_error
    );
}


TEST(FilesystemScannerTests, ConstructorRejectsFilePath) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_file";
    std::filesystem::create_directories(temp);

    auto file = temp / "test.txt";
    std::ofstream(file.string()) << "data";

    EXPECT_THROW(
        FilesystemScanner scanner(file),
        std::runtime_error
    );

    std::filesystem::remove_all(temp);
}


TEST(FilesystemScannerTests, ConstructorNormalizesToAbsolutePath) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_relative";
    std::filesystem::create_directories(temp);

    auto current = std::filesystem::current_path();
    std::filesystem::current_path(temp.parent_path());

    EXPECT_NO_THROW({
        FilesystemScanner scanner("scanner_relative");
    });

    std::filesystem::current_path(current);
    std::filesystem::remove_all(temp);
}


TEST(FilesystemScannerTests, ScanEmptyDirectoryProducesNoFiles) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_empty";
    std::filesystem::create_directories(temp);

    FilesystemScanner scanner(temp);
    ScanResult result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
    EXPECT_TRUE(result.errors.empty());

    std::filesystem::remove_all(temp);
}


TEST(FilesystemScannerTests, HiddenFilesExcludedByDefault) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_hidden";
    std::filesystem::create_directories(temp);

    auto file = temp / "hidden.txt";
    std::ofstream(file.string()) << "data";

    SetFileAttributesW(file.c_str(), FILE_ATTRIBUTE_HIDDEN);

    FilesystemScanner scanner(temp);
    auto result = scanner.scan();

    EXPECT_TRUE(result.files.empty());

    std::filesystem::remove_all(temp);
}

TEST(FilesystemScannerTests, FindSingleFile) {
    auto temp = std::filesystem::temp_directory_path() / "test_scan_dir";
    if (std::filesystem::exists(temp)) std::filesystem::remove_all(temp);
    std::filesystem::create_directories(temp);

    auto file = temp / "x.txt";
    std::ofstream(file) << "data";

    ASSERT_TRUE(std::filesystem::exists(file));

    FilesystemScanner scanner(temp);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_EQ(result.files[0].filename(), "x.txt");

    std::filesystem::remove_all(temp);
}

TEST(FilesystemScannerTests, FindsAllFiles) {
    auto temp = std::filesystem::temp_directory_path() / "find_files";
    std::filesystem::create_directories(temp);

    std::ofstream(temp / "a.txt") << "x";
    std::filesystem::create_directory(temp / "dir");
    std::ofstream(temp / "dir" / "b.txt") << "y";

    ScanOptions ops;
    ops.include_directories = true;
    FilesystemScanner scanner(temp, ops);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    EXPECT_EQ(result.files.size(), 3);

    std::filesystem::remove_all(temp);
}

TEST(FilesystemScannerTests, FileTypesAreCorrect) {
    auto temp = std::filesystem::temp_directory_path() / "scanner_types";
    std::filesystem::create_directories(temp);

    std::ofstream(temp / "a.txt") << "x";
    std::filesystem::create_directory(temp / "dir");

    ScanOptions ops;
    ops.include_directories = true;
    FilesystemScanner scanner(temp, ops);
    auto result = scanner.scan();

    bool saw_file = false;
    bool saw_dir = false;

    for (const auto& f : result.files) {
        if (f.type == FileType::RegularFile) saw_file = true;
        if (f.type == FileType::Directory) saw_dir = true;
    }

    EXPECT_TRUE(saw_file);
    EXPECT_TRUE(saw_dir);

    std::filesystem::remove_all(temp);
}


TEST(FilesystemScannerTests, RespectsMaximumDepth) {
    auto temp = std::filesystem::temp_directory_path() / "find_files";
    std::filesystem::create_directories(temp);

    std::ofstream(temp / "a.txt") << "x";
    std::filesystem::create_directory(temp / "dir");
    std::ofstream(temp / "dir" / "b.txt") << "y";

    ScanOptions ops;
    ops.max_depth = 0;
    FilesystemScanner scanner(temp, ops);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_EQ(result.files.at(0).filename(), "a.txt");

    std::filesystem::remove_all(temp);
}