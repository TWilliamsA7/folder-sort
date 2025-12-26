// tests/core/filesystem/FilesystemScannerTests.cpp

#include <gtest/gtest.h>
#include <fstream>
#include <chrono>

#include "core/filesystem/FilesystemScanner.hpp"
#include "../tests/helpers/TestFilesystem.hpp"
#include "../tests/core/filesystem/FilesystemScannerTest.hpp"

TEST_F(FilesystemScannerTest, ConstructorAcceptsValidDirectory) {
    auto temp = TestTree("scanner_valid");
    EXPECT_NO_THROW(FilesystemScanner scanner(temp.root));
}


TEST_F(FilesystemScannerTest, ConstructorRejectsNonExistentPath) {
    auto fake = std::filesystem::temp_directory_path() / "does_not_exist";

    EXPECT_THROW(
        FilesystemScanner scanner(fake),
        std::runtime_error
    );
}


TEST_F(FilesystemScannerTest, ConstructorRejectsFilePath) {
    auto tree = TestTree("scanner_file");
    auto file = tree.file("test.txt");

    EXPECT_THROW(
        FilesystemScanner scanner(file),
        std::runtime_error
    );
}


TEST_F(FilesystemScannerTest, ConstructorNormalizesToAbsolutePath) {
    auto temp = TestTree("scanner_relative");

    auto current = std::filesystem::current_path();
    std::filesystem::current_path(temp.root.parent_path());

    EXPECT_NO_THROW({
        FilesystemScanner scanner("scanner_relative");
    });

    std::filesystem::current_path(current);
}


TEST_F(FilesystemScannerTest, ScanEmptyDirectoryProducesNoFiles) {
    auto temp = TestTree("scanner_empty");

    FilesystemScanner scanner(temp.root);
    ScanResult result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
    EXPECT_TRUE(result.errors.empty());
}


TEST_F(FilesystemScannerTest, FindSingleFile) {
    auto temp = TestTree("test_scan_dir");
    auto file = temp.file("x.txt");

    ASSERT_TRUE(std::filesystem::exists(file));

    FilesystemScanner scanner(temp.root);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_EQ(result.files[0].filename(), "x.txt");
}


TEST_F(FilesystemScannerTest, FindsAllFiles) {
    auto temp = TestTree("find_files");
    temp.file("a.txt");
    auto br = temp.dir("dir");
    temp.file(br, "b.txt");

    ScanOptions ops;
    ops.include_directories = true;
    FilesystemScanner scanner(temp.root, ops);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    EXPECT_EQ(result.files.size(), 3);
}


TEST_F(FilesystemScannerTest, FileTypesAreCorrect) {
    auto temp = TestTree("find_files");
    temp.file("a.txt");
    auto br = temp.dir("dir");

    ScanOptions ops;
    ops.include_directories = true;
    FilesystemScanner scanner(temp.root, ops);
    auto result = scanner.scan();

    bool saw_file = false;
    bool saw_dir = false;

    for (const auto& f : result.files) {
        if (f.type == FileType::RegularFile) saw_file = true;
        if (f.type == FileType::Directory) saw_dir = true;
    }

    EXPECT_TRUE(saw_file);
    EXPECT_TRUE(saw_dir);
}


TEST_F(FilesystemScannerTest, RespectsMaximumDepth) {
    auto temp = TestTree("find_files");
    temp.file("a.txt");
    auto br = temp.dir("dir");
    temp.file(br, "b.txt");

    ScanOptions ops;
    ops.max_depth = 0;
    FilesystemScanner scanner(temp.root, ops);
    auto result = scanner.scan();

    EXPECT_FALSE(result.has_errors());

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_EQ(result.files.at(0).filename(), "a.txt");
}


TEST_F(FilesystemScannerTest, PathNormalizationDoesNotFailScan) {
    TestTree tree("scanner_norm");

    tree.file("a.txt");

    ScanOptions opts;
    opts.normalize_paths = true;

    FilesystemScanner scanner(tree.root, opts);
    auto result = scanner.scan();

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_TRUE(result.files[0].path.is_absolute());
}

TEST_F(FilesystemScannerTest, SmallTreeCompletesQuickly) {
    TestTree tree("scanner_time");

    for (int i = 0; i < 50; ++i)
        tree.file("f" + std::to_string(i));

    auto start = std::chrono::steady_clock::now();

    FilesystemScanner scanner(tree.root);
    scanner.scan();

    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_LT(elapsed, std::chrono::milliseconds(100));
}

TEST_F(FilesystemScannerTest, CreatesLogs) {
    auto temp = TestTree("find_files");
    temp.file("a.txt");
    auto br = temp.dir("dir");
    temp.file(br, "b.txt");

    ScanOptions ops;
    ops.include_directories = true;
    ops.max_depth = 0;
    FilesystemScanner scanner(temp.root, ops);
    auto result = scanner.scan();

    ASSERT_TRUE(std::filesystem::exists(logFile));
}
