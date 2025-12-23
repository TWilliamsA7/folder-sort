// tests/core/filesystem/FilesystemScannerTests.cpp

#include <gtest/gtest.h>
#include <fstream>
#include <chrono>

#include "core/filesystem/FilesystemScanner.hpp"
#include "../tests/helpers/TestFilesystem.hpp"

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

TEST(FilesystemScannerTests, MaxDepthStopsTraversal) {
    TestTree tree("scanner_depth");

    auto d1 = tree.dir("level1");
    auto d2 = std::filesystem::create_directories(d1 / "level2");
    std::ofstream(d1 / "level2" / "deep.txt") << "x";

    ScanOptions opts;
    opts.max_depth = 0;

    FilesystemScanner scanner(tree.root, opts);
    auto result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
}

TEST(FilesystemScannerTests, HiddenDirectoryIsPruned) {
    TestTree tree("scanner_hidden");

    auto hidden = tree.dir("hidden");
    std::ofstream(hidden / "inside.txt") << "x";

    SetFileAttributesW(hidden.c_str(), FILE_ATTRIBUTE_HIDDEN);

    ScanOptions opts;
    opts.include_hidden = false;

    FilesystemScanner scanner(tree.root, opts);
    auto result = scanner.scan();

    EXPECT_TRUE(result.files.empty());
}

TEST(FilesystemScannerTests, SymlinkNotFollowed) {
    TestTree tree("scanner_symlink");

    auto real = tree.dir("real");
    std::ofstream(real / "file.txt") << "x";

    auto link = tree.root / "link";
    CreateSymbolicLinkW(link.c_str(), real.c_str(), SYMBOLIC_LINK_FLAG_DIRECTORY);

    ScanOptions opts;
    opts.follow_symlinks = false;

    FilesystemScanner scanner(tree.root, opts);
    auto result = scanner.scan();

    EXPECT_EQ(result.files.size(), 1);
}

TEST(FilesystemScannerTests, PathNormalizationDoesNotFailScan) {
    TestTree tree("scanner_norm");

    tree.file("a.txt");

    ScanOptions opts;
    opts.normalize_paths = true;

    FilesystemScanner scanner(tree.root, opts);
    auto result = scanner.scan();

    ASSERT_EQ(result.files.size(), 1);
    EXPECT_TRUE(result.files[0].path.is_absolute());
}

TEST(FilesystemScannerTests, SmallTreeCompletesQuickly) {
    TestTree tree("scanner_time");

    for (int i = 0; i < 50; ++i)
        tree.file("f" + std::to_string(i));

    auto start = std::chrono::steady_clock::now();

    FilesystemScanner scanner(tree.root);
    scanner.scan();

    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_LT(elapsed, std::chrono::milliseconds(50));
}


